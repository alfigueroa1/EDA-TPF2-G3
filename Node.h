#pragma once

#include "subject.h"
#include "blockChain.h"
#include "errorType.h"

#include <iostream>
using namespace std;

typedef string ID;

//ALEX WAS HERE:
struct sSocket {
	sSocket() : ip(), port() {}
	sSocket(const string& _ip, const string& _port) : ip(_ip), port(_port) {}
	sSocket& operator=(const sSocket& s) { ip = s.ip; port = s.port; return *this; }
	//sSocket(size_t s, char c, size_t s2, char c2) : ip(s,c),port(s2,c2){}	//punteros
	string ip;
	string port;
};


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
	//Server
	vector <Client> clients;
	unsigned int port;

	string createJsonBlock(unsigned int height);
	string createJsonTx(Transaction tx);
	string createJsonMerkle();
	string createJsonFilter(string filter);
	string createHeader(unsigned int height);

	string createServerErrRsp();
	string createServerBlock(string path);
	string createServerOkRsp(string path);
	void createDates(char*, char*);

	void addBlock(Block block);

	// solo para evitar su uso
public:
	Node& operator=(Node& n) = delete;
	//Node(Node& n);
};