#pragma once

#include "subject.h"
#include "blockChain.h"
#include "errorType.h"

typedef string ID;

struct sSocket {
	//sSocket() : ip(), port() {}
	//sSocket(const string& _ip, const string& _port) : ip(_ip), port(_port) {}
	//sSocket& operator=(const sSocket& s) { ip = s.ip; port = s.port; return *this; }
	string ip;
	string port;
};

class Node : public Subject {
public:
	Node(string _ID, string _port, string _IP);
	~Node();
	errorType postBlock(unsigned int neighbourPos);
	errorType getBlockHeader(unsigned int height, unsigned int neighbourPos);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postMerkleBlock(unsigned int neighbourPos);		//Posts about last block in blockchain
	errorType postFilter(unsigned int neighbourPos);
	errorType AddNeighbour(const string& _ID,const string& _port);

	ID getID();
	string getIP() { return IP; }
	string getPort() { return to_string(port); }
	const vector<sSocket>* getNeighbours() { return &neighbourhood; }
	const vector<Transaction>* getTransactions() { return &txs; };
	const vector<string>* getFilters() { return &filters; };
	void keepListening();
	void keepSending();

	void debugTx(const Transaction& _tx) { txs.emplace_back(_tx); notifyAllObservers(); }
	
private:
	
	void addBlock(Block block);
	
	string myID;
	string IP;
	unsigned int port;
	BlockChain chain;
	//Server
	//Client vector
	vector<BlockChain> dummieChain;
	vector<sSocket> neighbourhood;
	vector<string> filters;
	vector<Transaction> txs;
};