/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "blockChainModel.h"

 /*******************************************************************************
  * FUNCTION DECLARATIONS WITH FILE LEVEL SCOPE
  ******************************************************************************/
static unsigned int generateID(const char* str);
 /*******************************************************************************
  * CLASS METHODS DEFINITIONS
  ******************************************************************************/

BlockChainModel::BlockChainModel(void) : blockChain(), selectedBlock(false)
{

}

BlockChainModel::~BlockChainModel(void)
{

}

vector<Block>::iterator BlockChainModel::getCurr(void) { return curr; }
unsigned long int BlockChainModel::getNumberOfBlocks(void) { return blockChain.size(); }

vector<string>* BlockChainModel::getBlockChainNames(string path) 
{ 
	selectedBlock = false;
	notifyAllObservers();
	return finder.getValidJSONs(path); 
}

void BlockChainModel::clearBlockChain()
{
	blockChain.clear();
}


void BlockChainModel::openBlockChain(string path) 
{
	clearBlockChain();
	finder.saveBlockChain(blockChain, path);
	selectedBlock = false;
	notifyAllObservers();
}

void BlockChainModel::openBlock(unsigned long int b) {
	bool found = false;
	for (auto i = blockChain.begin(); i != blockChain.end(); i++) {
		if (i->getHeight() == b) {
			curr = i;
			found = true;
		}
	}
	if (!found) {
		curr = blockChain.end();
		return;
	}
	tree.blockId = curr->blockId;
	tree.height = curr->height;
	tree.nonce = curr->nonce;
	tree.nTx = curr->nTx;
	tree.previousBlockId = curr->previousBlockId;
	//Missing: tree.merkleRoot, tree.tree. getMerkleTree takes care of that.
	if (curr->nTx == 0) {
		tree.tree.clear();
		tree.merkleRoot = "";
	}
	else if (curr->nTx == 1) {
		char aux[9];
		unsigned int ID = generateID(curr->tx[0].txId.c_str());
		sprintf_s(&aux[0], 9, "%08X", ID);
		newIDstr newID(aux);
		tree.tree.clear();
		tree.tree.push_back(newID);
		tree.merkleRoot = newID;
	}
	else {
		tree.tree.clear();
		getMerkleTree();
	}
	selectedBlock = true;
	notifyAllObservers();
}

const MerkleTree* BlockChainModel::getOpenTree() {
	if (selectedBlock == true && !blockChain.empty() && curr != blockChain.end()) {
		return &tree;
	}
	else
		return nullptr;
}

void BlockChainModel::getMerkleTree() {
	unsigned long int transactions = curr->nTx;
	unsigned long int nearestPowerOf2 = pow(2, ceil(log(transactions) / log(2)));
	//Normalize floor of tree
	//Not filling floor
	for (int i = 0; i < transactions; i++) {
		//tree.tree.push_back(curr->tx[i].vIn.txId);
		unsigned long int inTrans = curr->tx[i].nTxIn;
		string concatenate;
		concatenate.clear();
		for (int j = 0; j < inTrans; j++)
			concatenate += curr->tx[i].vIn[j].txId;
		char aux[9];
		unsigned int ID = generateID(concatenate.c_str());
		sprintf_s(&aux[0], 9, "%08X", ID);
		newIDstr leafID(aux);
		tree.tree.push_back(leafID);
	}
	int prevLvlAmount = transactions;
	if (transactions % 2) {
		tree.tree.push_back(*(tree.tree.end()-1));
		prevLvlAmount++;
	}
	int maxLvl = (log(nearestPowerOf2) / log(2));		//Max level es el nivel del arbol menos el MerkleRoot
	for (int i = maxLvl - 1; i > 0; i--) {					//Itera por cada nivel del arbol menos el primero que ya esta normalizado
		vector<newIDstr>::iterator it = tree.tree.end();
		fillLevel(i, &prevLvlAmount, it);				//Rellena todos los arboles
	}

	newIDstr concatenate = *(tree.tree.end()-2) + *(tree.tree.end()-1);		//Concatena un par de elementos del nivel anterior
	char aux[9];
	unsigned int ID = generateID(concatenate.c_str());
	sprintf_s(&aux[0],9, "%08X", ID);
	newIDstr newID(aux);
	tree.tree.push_back(newID);

	tree.merkleRoot = curr->merkleRoot;
}

void BlockChainModel::fillLevel(int level, int* prevLvlAmount, vector<newIDstr>::iterator it) {		//Fill level asume que el nivel anterior esta completo y lindo
	vector<newIDstr> lvl;
	lvl.clear();
	vector<newIDstr>::iterator newIt = it;						//it esta posicionado en el ultimo elemento del nivel anterior
	newIt -= (*prevLvlAmount);								//newIt apunta al primer elemento del nivel anterior
	for (int i = 0; i < *prevLvlAmount; i+= 2) {
		newIDstr concatenate = *(newIt + i) + *(newIt + i + 1);		//Concatena un par de elementos del nivel anterior
		char aux[9];
		unsigned int ID = generateID(concatenate.c_str());
		sprintf_s(&aux[0],9, "%08X", ID);
		newIDstr newID(aux);
		lvl.push_back(newID);										//Se pushea la concatenacion al vector del nivel actual
	}
	*prevLvlAmount /= 2;											//Ahora el nivel actual tiene la mitad de elementos que el anterior
	if ((*prevLvlAmount) % 2) {										//Si el nivel actual tiene elementos impares
		lvl.push_back(*(lvl.end()-1));								//Se copia el ultimo elemento
		(*prevLvlAmount)++;											//El nivel actual queda con elementos pares
	}
	for (auto i = lvl.begin(); i != lvl.end(); i++) {
		tree.tree.push_back(*i);									//Se pushea todo el nivel al arbol
	}
	return;
}


/*******************************************************************************
 * FUNCTION DEFINITIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static unsigned int generateID(const char* str) {
	unsigned int ID = 0;
	int c;
	while (c = *str++)
		ID = c + (ID << 6) + (ID << 16) - ID;

	return ID;
}