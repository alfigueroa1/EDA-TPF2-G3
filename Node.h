#pragma once

#include "subject.h"
#include "blockChain.h"

typedef bool errorType;
typedef string ID;

//ALEX WAS HERE:
struct sSocket {
	sSocket() : ip(), port() {}
	sSocket(const string& _ip,const string& _port) : ip(_ip), port(_port) {}
	string ip;
	string port;
};


class Node : public Subject {
public:
	Node(string _ID, string _port, string _IP);		//ALEX
	~Node();
	errorType postBlock(unsigned int neighbourPos);
	errorType getBlockHeader(unsigned int height, unsigned int neighbourPos);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postMerkleBlock(unsigned int neighbourPos);		//Posts about last block in blockchain
	errorType postFilter(unsigned int neighbourPos);
	void AddNeighbour(const string& _ID,const string& _port);

	ID getID();
	string getIP() { return IP; }
	string getPort() { return to_string(port); }
	const vector<sSocket>* getNeighbours() { return &neighbourhood; }; //ALEX WAS HERE AGAIN
	const vector<Transaction>* getTransactions() { return &txs; };
	const vector<string>* getFilters() { return &filters; };
	void keepListening();
	void keepSending();
	
private:
	string myID;			//ALEX
	void addBlock(Block block);
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