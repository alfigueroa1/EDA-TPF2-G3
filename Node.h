#pragma once

#include "subject.h"
#include "blockChain.h"

#include <iostream>
using namespace std;

typedef bool errorType;
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
	Node(string _ID, string _port, string _IP);		//ALEX
	~Node();
	errorType postBlock(unsigned int neighbourPos);
	errorType getBlockHeader(unsigned int height, unsigned int neighbourPos);
	errorType postTransaction(unsigned int neighbourPos, Transaction tx);
	errorType postMerkleBlock(unsigned int neighbourPos);		//Posts about last block in blockchain
	errorType postFilter(unsigned int neighbourPos);
	void AddNeighbour(const string& _ID,const string& _port);

	ID getID();
	string getIP() { return *IP; } //punteros
	string getPort() { return to_string(port); }
	const vector<sSocket>* getNeighbours() { return neighbourhood; }; //ALEX WAS HERE AGAIN //punteros
	const vector<Transaction>* getTransactions() { return &txs; };
	const vector<string>* getFilters() { return &filters; };
	void keepListening();
	void keepSending();


	//DEBUG
	void debugTx(const Transaction& TX) {
		txs.emplace_back(TX);
		notifyAllObservers();

	}

	
private:
	string* myID;			//ALEX //punterros
	void addBlock(Block block);
	string* IP; //punteros
	unsigned int port;
	BlockChain chain;
	//Server
	//Client vector
	vector<BlockChain> dummieChain;
	vector<sSocket>* neighbourhood; //punteros
	vector<string> filters;
	vector<Transaction> txs;



	// solo para evitar su uso
public:
	Node& operator=(Node& n) = delete;
	//Node(Node& n);
};