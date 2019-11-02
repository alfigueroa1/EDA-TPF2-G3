#pragma once

#include "subject.h"
#include "blockChain.h"

typedef bool errorType;
typedef string ID;

class Node : public Subject {
public:
	Node();
	~Node();
	errorType postBlock(unsigned int neighbourPos);
	errorType getBlockHeader(unsigned int height, unsigned int neighbourPos);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postMerkleBlock(unsigned int neighbourPos);		//Posts about last block in blockchain
	errorType postFilter(unsigned int neighbourPos);
	void AddNeighbour(string _ID, string _port);

	ID getID();
	string getIP() { return IP; }
	string getPort() { return to_string(port); }
	vector<string> getNeighbours();
	vector<Transaction>* getTransactions() { return &txs; };
	vector<string>* getFilters() { return &filters; };
	void keepListening();
	void keepSending();
	
private:
	void addBlock(Block block);
	string IP;
	unsigned int port;
	BlockChain chain;
	//Server
	//Client vector
	vector<BlockChain> dummieChain;
	vector<string> neighbourhood;
	vector<string> filters;
	vector<Transaction> txs;
};