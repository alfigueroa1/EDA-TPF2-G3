#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "subject.h"
#include "blockChain.h"
#include "errorType.h"
#include "sSocket.h"
#include "Client.h"
#include "Server.h"
#include <iostream>


using namespace std;

typedef string ID;

class Node : public Subject {
public:
	Node(string _ID, string _port, string _IP);	
	~Node();

	errorType postBlock(unsigned int neighbourPos, unsigned int height);
	errorType getBlockHeader(unsigned int height, unsigned int neighbourPos);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postMerkleBlock(unsigned int neighbourPos);
	errorType postFilter(unsigned int neighbourPos);
	errorType AddNeighbour(const string& _ID,const string& _port);

	ID getID();
	string getIP() { return *IP; } 
	string getPort() { return to_string(port); }
	const vector<sSocket>* getNeighbours();
	const vector<Transaction>* getTransactions();
	const vector<string>* getFilters();
	void keepListening();
	void keepSending();


	//DEBUG
	void debugTx(const Transaction& TX) {
		txs.emplace_back(TX);
		notifyAllObservers();

	}

	
private:
	string* myID;			
	string* IP; 
	BlockChain chain;
	BlockChain dummieChain;
	vector<sSocket>* neighbourhood; //punteros
	vector<string> filters;
	vector<Transaction> txs;
	vector <Server> servers;
	vector <Client> clients;
	unsigned int port;

	void addBlock(Block block);

	string createJsonBlock(unsigned int height);
	string createJsonTx(Transaction tx);
	string createJsonMerkle();
	string createJsonFilter(string filter);
	string createHeader(unsigned int height);

	string createServerErrRsp();
	string createServerBlock(string path);
	string createServerOkRsp(string path);
	void createDates(char*, char*);

	// solo para evitar su uso
public:
	Node& operator=(Node& n) = delete;
	//Node(Node& n);
};