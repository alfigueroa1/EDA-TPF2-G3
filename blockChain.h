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
	//Vin(const Vin& v) : //innecesario
	//	blockId(v.blockId), txId(v.txId){}
	//Vin(string& s1, string& s2) :
	//	blockId(s1), txId(s2){}
	string blockId;
	string txId;
};
struct Vout{
	//Vout(const Vout& v) :
	//	publicId(v.publicId),amount(v.amount) {}
	//Vout(string& s, unsigned long int& i) :
	//	publicId(s), amount(i) {}
	string publicId;
	unsigned long int amount;
};
struct Transaction{
	//Transaction(const Transaction&t) :
	//	txId(t.txId), nTxIn(t.nTxIn),nTxOut(t.nTxOut),
	//	vIn(t.vIn),vOut(t.vOut){}
	//Transaction() :
	//	txId(), nTxIn(), nTxOut(),
	//	vIn(), vOut() {}
	//Transaction(const string& id,unsigned long int& in,
	//	vector<Vin>& _vin, unsigned long int& out,
	//	vector<Vout>& _vout):
	//	txId(id),nTxIn(in),vIn(_vin),nTxOut(out),vOut(_vout){}
	string txId;
	unsigned long int nTxIn;
	vector<Vin> vIn;
	unsigned long int nTxOut;
	vector<Vout> vOut;
};



//DUMMY TX //terminar constructores...
//const string dummy_txId = "DUMMY TX";
//const vector<Vin> dummyIn = { Vin(string("BLOCK ID 1"),string("TX ID 1")),Vin("BLOCK ID 2","TX ID 2") };
const Transaction dummyTX = { "DUMMY",2,{{"A","B"},{"C","D"}},3,{{"ALE",20},{"GIAN",30},{"OLI",40}} };



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