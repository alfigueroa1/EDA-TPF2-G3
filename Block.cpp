/******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <vector>
#include "Block.h"
#include "json.hpp"
#include <fstream>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLOCK_FIELDS 7
#define TRANS_FIELDS 5

 /*******************************************************************************
  * FUNCTION DECLARATIONS WITH FILE LEVEL SCOPE
  ******************************************************************************/
static unsigned int generateID(const char* str);


/*******************************************************************************
 * CLASS METHODS DEFINITIONS
 ******************************************************************************/
using namespace std;
using json = nlohmann::json;


bool Block::validateBlock(string blck)
{
	bool ret = false;

	try
	{
		json block = json::parse(blck);

		//Block
		if (block.size() == BLOCK_FIELDS) //Si son 7 elementos
		{
			block.at("height");
			block.at("nonce");
			block.at("blockid");	//Se fija que sean los correspondientes
			block.at("previousblockid");
			block.at("merkleroot");
			int ntx = block.at("nTx");
			block.at("height");
			block.at("tx");

			//Transactions
			auto arrayTrans = block["tx"];
			for (auto& trans : arrayTrans)	//Parsea todas las transacciones
			{
				if (arrayTrans.size() == ntx && trans.size() == TRANS_FIELDS)	//Si son 5 elementos
				{
					trans.at("txid");
					int txin = trans.at("nTxin");
					trans.at("vin");	//Se fija que sean los correctos
					int txout = trans.at("nTxout");
					trans.at("vout");

					auto vIn = trans["vin"];
					auto vOut = trans["vout"];
					if (vIn.size() == txin && vOut.size() == txout)
					{
						for (auto& elsi : vIn)
						{
							elsi.at("blockid");
							elsi.at("txid");
						}

						for (auto& elso : vOut)
						{
							elso.at("publicid");
							elso.at("amount");
						}

						ret = true;
					}
				}
			}
		}
	}

	catch (std::exception& e)
	{
		ret = false;
	}

	return ret;
}


vector<newIDstr> Block::getMerklePath(Transaction trx) {
	vector<newIDstr> path;
	vector<newIDstr> tree = getMerkleTree();
	path.clear();
	auto i = tx.begin();
	int j = 0;
	for (; i != tx.end(); i++, j++) {
		if (i->txId == trx.txId)
			break;
	}
	if (i != tx.end()) {	//j CONTAINS TRANSACTION POSITION IN tx ARRAY
		fillMerklePath(&path, &tree, j);
	}
	return path;
}

void Block::fillMerklePath(vector<newIDstr>* path, vector<newIDstr>* tree, int j){
	unsigned long int transactions = nTx;
	unsigned long int nearestPowerOf2 = pow(2, ceil(log(transactions) / log(2)));
	int maxLvl = (log(nearestPowerOf2) / log(2));		//Max level es el nivel del arbol menos el MerkleRoot

	if (j % 2) {										//Si el txID ocupa una posicon impar
		path->push_back((*tree)[j - 1]);				//Cargo primero el anterior
		//path->push_back((*tree)[j]);					//Y luego el actual
	}
	else {												//Si el txID es par
		//path->push_back((*tree)[j]);					//Cargo primero el actual
		path->push_back((*tree)[j + 1]);				//Luego el siguiente
	}

	int prevLvlAmount = nTx;
	if (nTx % 2)
		prevLvlAmount++;
	int offset = prevLvlAmount;
	j /= 2;
	while (--maxLvl) {
		if (j % 2)												//Si el txID ocupa una posicon impar
			path->push_back((*tree)[offset + j - 1]);			//Cargo el anterior
		else													//Si el txID es par
			path->push_back((*tree)[offset + j + 1]);			//Cargo el siguiente
		j /= 2;
		prevLvlAmount /= 2;										//Ahora el nivel actual tiene la mitad de elementos que el anterior
		if (prevLvlAmount % 2)
			prevLvlAmount++;
		offset += prevLvlAmount;
	}

	//path->push_back(*tree->end())								//Uncomment to push merkle root
	return;
}

long int Block::getBlockPos(vector<Block>* BlockChain) {
	if (!BlockChain->empty()) {
		auto i = BlockChain->begin();
		for (int j = 0; i != BlockChain->end(); i++, j++) {
			if (i->getHeight() == height)
				return j;
		}
	}
	return -1;
}

vector<newIDstr> Block::getMerkleTree() {
	vector<string> tree;
	tree.clear();
	unsigned long int transactions = nTx;
	unsigned long int nearestPowerOf2 = pow(2, ceil(log(transactions) / log(2)));
	//Normalize floor of tree
	//Not filling floor
	for (int i = 0; i < transactions; i++) {
		unsigned long int inTrans = tx[i].nTxIn;
		string concatenate;
		concatenate.clear();
		for (int j = 0; j < inTrans; j++)
			concatenate += tx[i].vIn[j].txId;
		char aux[9];
		unsigned int ID = generateID(concatenate.c_str());
		sprintf_s(&aux[0], 9, "%08X", ID);
		newIDstr leafID(aux);
		tree.push_back(leafID);
	}
	int prevLvlAmount = transactions;
	if (transactions % 2) {
		tree.push_back(*(tree.end() - 1));
		prevLvlAmount++;
	}
	int maxLvl = (log(nearestPowerOf2) / log(2));		//Max level es el nivel del arbol menos el MerkleRoot
	for (int i = maxLvl - 1; i > 0; i--) {					//Itera por cada nivel del arbol menos el primero que ya esta normalizado
		vector<newIDstr>::iterator it = tree.end();
		fillLevel(i, &prevLvlAmount, it, &tree);				//Rellena todos los arboles
	}

	newIDstr concatenate = *(tree.end() - 2) + *(tree.end() - 1);		//Concatena un par de elementos del nivel anterior
	char aux[9];
	unsigned int ID = generateID(concatenate.c_str());
	sprintf_s(&aux[0], 9, "%08X", ID);
	newIDstr newID(aux);
	tree.push_back(newID);

	return tree;
}

void Block::fillLevel(int level, int* prevLvlAmount, vector<newIDstr>::iterator it, vector<newIDstr>* tree) {		//Fill level asume que el nivel anterior esta completo y lindo
	vector<newIDstr> lvl;
	lvl.clear();
	vector<newIDstr>::iterator newIt = it;						//it esta posicionado en el ultimo elemento del nivel anterior
	newIt -= (*prevLvlAmount);								//newIt apunta al primer elemento del nivel anterior
	for (int i = 0; i < *prevLvlAmount; i += 2) {
		newIDstr concatenate = *(newIt + i) + *(newIt + i + 1);		//Concatena un par de elementos del nivel anterior
		char aux[9];
		unsigned int ID = generateID(concatenate.c_str());
		sprintf_s(&aux[0], 9, "%08X", ID);
		newIDstr newID(aux);
		lvl.push_back(newID);										//Se pushea la concatenacion al vector del nivel actual
	}
	*prevLvlAmount /= 2;											//Ahora el nivel actual tiene la mitad de elementos que el anterior
	if ((*prevLvlAmount) % 2) {										//Si el nivel actual tiene elementos impares
		lvl.push_back(*(lvl.end() - 1));								//Se copia el ultimo elemento
		(*prevLvlAmount)++;											//El nivel actual queda con elementos pares
	}
	for (auto i = lvl.begin(); i != lvl.end(); i++) {
		tree->push_back(*i);									//Se pushea todo el nivel al arbol
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