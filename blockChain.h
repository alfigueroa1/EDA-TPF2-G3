#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <string>
#include <vector>

using namespace std;

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
struct Vin{
	string blockId;
	string txId;
};
struct Vout{
	string publicId;
	unsigned long int amount;
};
struct Transaction{
	string txId;
	unsigned long int nTxIn;
	vector<Vin> vIn;
	unsigned long int nTxOut;
	vector<Vout> vOut;
};



//DUMMY TX
const Transaction dummyTX = { "DUMMY TX",2,{{"BLOCK ID 1","TX ID 1"},{"BLOCK ID 2","TX ID 2"}},3,{{"ALE",20},{"GIAN",30},{"OLI",40}} };



typedef std::string newIDstr;	//Cambie de parecer. Q los IDs sean strings
typedef unsigned long int longN;
typedef vector<newIDstr> MerkleTree;

class Block {
public:
	vector<Transaction> getTransactions() { return tx; };
	Transaction getTx(vector<Transaction>::iterator it) { for (auto i = tx.begin(); i != tx.end(); i++) { if (i == it) return *i; } };
	Transaction getTx(unsigned int it) { if (it >= 0 && it < tx.size())	return tx[it]; };
	unsigned long int getHeight() { return height; };
	unsigned long int getNonce() { return nonce; };
	string getBlockID() { return blockId; };
	string getPreviousBlockID() { return previousBlockId; };
	string getMerkleRoot() { return merkleRoot; };
	unsigned long int getNTx() { return nTx; };

private:
	vector<Transaction> tx;
	unsigned long int height;
	unsigned long int nonce;
	string blockId;
	string previousBlockId;
	string merkleRoot;
	unsigned long int nTx;
};

typedef vector<Block> BlockChain;