/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Node.h"
#include "json.hpp"
#include <iostream>
#include <fstream>

/*******************************************************************************
 * NAMESPACES
 ******************************************************************************/
using namespace std;
using json = nlohmann::json;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CRLF "\x0D\x0A"


/*******************************************************************************
 * CASS METHODS DEFINITIONS
 ******************************************************************************/

//CONSTRUCTOR
Node::Node(string _ID, string _port, string _IP) { //constructor modificado para trabajar con punteros
	port = stoi(_port);
	myID = new string(40,' ');
	IP = new string(40,' ');
	*myID = _ID;
	*IP = _IP;
	//Fill bblockchain
	saveBlockChain(dummieChain, "dummy.json");
	//Create Genesis Server
	Server* genesisServer = new Server(port);
	genesisServer->startConnection();			//Preguntar si esto funcaria
	servers.push_back(genesisServer);
	cout << "Created (the good way)" << endl;
}

//DESTRUCTOR
Node::~Node() { // igual el destructor

	cout << "Destroyed node" << endl;

	delete myID;
	delete IP;

	if (!servers.empty()) {
		for (int i = 0; i < servers.size(); i++)
			delete servers[i];
		servers.clear();
	}
	if (!clients.empty()) {
		for (int i = 0; i < clients.size(); i++)
			delete clients[i];
		clients.clear();
	}
}

//EDANET
errorType Node::AddNeighbour(const string& _IP, const string& _port) {	//ALEX!!!	//punteros
	neighbourhood.emplace_back(_IP, _port);
	notifyAllObservers();

	errorType error;
	error.error = false;
	error.datos = "";

	if (stoi(_port) < 0) {
		error.error = true;
		error.datos = "No querido, que clase de puerto es ese? Te fumaste algo fuerte?";
	}

	return error;
}

void Node::saveBlockChain(BlockChain& blockchain, string path)
{
	std::ifstream i(path.c_str()); //Se puede cambiar, no se como recibo el JSON;
	json j;
	i >> j;

	for (auto& blocks : j)
	{
		//Block 
		Block block;

		auto height = blocks["height"];
		block.setHeight(height);

		auto nonce = blocks["nonce"];
		block.setNonce(nonce);

		auto blockId = blocks["blockid"];
		block.setBlockId(blockId.get<string>());

		auto prevBlockId = blocks["previousblockid"];
		block.setPrevBlockId(prevBlockId.get<string>());

		auto root = blocks["merkleroot"];
		block.setMerkleRoot(root.get<string>());

		auto nTx = blocks["nTx"];
		block.setNTx(nTx);

		//Transactions
		auto arrayTrans = blocks["tx"];
		for (auto& trans : arrayTrans)
		{
			Transaction auxTrans;

			auto txId = trans["txid"];
			auxTrans.txId = txId.get<string>();

			auto nTxIn = trans["nTxin"];
			auxTrans.nTxIn = nTxIn;

			auto vIn = trans["vin"];
			for (auto& elsi : vIn)
			{
				Vin auxVin;

				auto tBlockId = elsi["blockid"];
				auxVin.blockId = tBlockId.get<string>();

				auto tTxId = elsi["txid"];
				auxVin.txId = tTxId.get<string>();

				auxTrans.vIn.push_back(auxVin);
			}

			auto nTxOut = trans["nTxout"];
			auxTrans.nTxOut = nTxOut;

			auto vOut = trans["vout"];
			for (auto& elso : vOut)
			{
				Vout auxVout;

				auto publicId = elso["publicid"];
				auxVout.publicId = publicId.get<string>();

				auto amount = elso["amount"];
				auxVout.amount = amount;

				auxTrans.vOut.push_back(auxVout);
			}

			block.addTx(auxTrans);
		}

		blockchain.push_back(block);
	}
}

void Node::keepListening()
{
	vector<vector<Server*>::iterator> deleteThis;
	vector<Server*> doneServers;
	servers.back()->listening();

	if ((*(servers.end() - 1))->getDoneListening()) {
		cout << "Latest Server picked up something!" << endl;
		Server* newServer = new Server(port);
		newServer->startConnection();
		servers.push_back(newServer);
		cout << "New Server created and pushed!" << endl;
	}
	auto i = servers.begin();
	for (; i != servers.end()-1; i++) {
		if (!(*i)->getDoneDownloading())
			(*i)->receiveMessage();
		else if (!(*i)->getDoneSending())
			(*i)->sendMessage(serverResponse((*i)->getState()));
		if ((*i)->getDoneSending()) {
			cout << "Server done servering" << endl;
			doneServers.push_back(*i);
			deleteThis.push_back(i);
		}
	}
	//Handle finished servers
	auto j = doneServers.begin();
	for (; j != doneServers.end(); j++) {
		Block blck;
		switch ((*j)->getState()){
		case BLOCK:			//Done
			blck.saveBlock((*j)->getMessage());
			chain.push_back(blck);
			break;
		case TX:			//Done
			saveTx((*j)->getMessage());
			break;
		case MERKLE:		//Done
			//Merkle mkl;
			saveMerkleBlock((*j)->getMessage());
			break;
		case FILTER:		//Done
			filters.push_back((*j)->getMessage());
			break;
		}
	}
	i = servers.begin();
	for (; i != servers.end() - 1; i++) {
		if ((*i)->getDoneSending())
			delete* i;
	}
	auto k = deleteThis.begin();
	for (; k != deleteThis.end(); k++) {
		servers.erase(*k);
	}
	if (!deleteThis.empty())
		notifyAllObservers();
}

void Node::saveMerkleBlock(string _merkleBlock) {
	MerkleBlock mBlock;
	json merkleBlock = json::parse(_merkleBlock);

	auto blockId = merkleBlock["blockid"];
	mBlock.blockId = blockId.get<string>();

	auto mPath = merkleBlock["merklePath"];
	for (auto& elsi : mPath) {
		auto Id = elsi["Id"];
		mBlock.merklePath.push_back(Id.get<string>());
	}

	//Transactions
	auto trans = merkleBlock["tx"];
	Transaction auxTrans;

	auto txId = trans["txid"];
	auxTrans.txId = txId.get<string>();

	auto nTxIn = trans["nTxin"];
	auxTrans.nTxIn = nTxIn;

	auto vIn = trans["vin"];
	for (auto& elsi : vIn)
	{
		Vin auxVin;

		auto tBlockId = elsi["blockid"];
		auxVin.blockId = tBlockId.get<string>();

		auto tTxId = elsi["txid"];
		auxVin.txId = tTxId.get<string>();

		auxTrans.vIn.push_back(auxVin);
	}

	auto nTxOut = trans["nTxout"];
	auxTrans.nTxOut = nTxOut;

	auto vOut = trans["vout"];
	for (auto& elso : vOut)
	{
		Vout auxVout;

		auto publicId = elso["publicid"];
		auxVout.publicId = publicId.get<string>();

		auto amount = elso["amount"];
		auxVout.amount = amount;

		auxTrans.vOut.push_back(auxVout);
	}

	mBlock.tx.push_back(auxTrans);
	auto txPos = merkleBlock["txPos"];
	mBlock.txPos = txPos;
	

	merkleBlocks.push_back(mBlock);
}

void Node::saveTx(string _trans) {
	json trans;
	Transaction tx;
	trans = json::parse(_trans);
	auto txId = trans["txid"];
	tx.txId = txId.get<string>();
	auto nTxIn = trans["nTxin"];
	tx.nTxIn = nTxIn;
	auto vIn = trans["vin"];
	for (auto& elsi : vIn) {
		Vin auxVin;
		auto tBlockId = elsi["blockid"];
		auxVin.blockId = tBlockId.get<string>();
		auto tTxId = elsi["txid"];
		auxVin.txId = tTxId.get<string>();
		tx.vIn.push_back(auxVin);
	}
	auto nTxOut = trans["nTxout"];
	tx.nTxOut = nTxOut;
	auto vOut = trans["vout"];
	for (auto& elso : vOut) {
		Vout auxVout;
		auto publicId = elso["publicid"];
		auxVout.publicId = publicId.get<string>();
		auto amount = elso["amount"];
		auxVout.amount = amount;
		tx.vOut.push_back(auxVout);
	}
	txs.push_back(tx);
}

void Node::keepSending()
{
	for (int i = 0; i < clients.size(); i++) {
		clients[i]->sendRequest();
		if (clients[i]->getRunning() == 0) {
			//FULLNODES DONT EXPECT ANSWER TO POSTS IN NETWORK_READY STATE (?)
			if (clients[i]->getClientType() == POSTClient) {

			}
			else if (clients[i]->getClientType() == GETClient) {
				saveMerkleBlock(clients[i]->getResponse());
			}
			delete clients[i];								//Destroy client
			clients.erase(clients.begin() + i);				//Remove client from list
			notifyAllObservers();
		}
	}
}

void Node::addBlock(Block block) 
{
	chain.push_back(block);
}

//POST-GET
errorType Node::postBlock(unsigned int neighbourPos, unsigned int height)
{
	errorType err = { false,"" };
	Client* client = new Client(neighbourhood[neighbourPos]);
	string blck = createJsonBlock(height);
	client->POST("/eda_coin/send_block", blck);
	client->sendRequest();
	clients.push_back(client);
	cout << "Created Client" << endl;		//DEBUG
	notifyAllObservers();
	return err;
}

errorType Node::getBlockHeader(unsigned int height, unsigned int neighbourPos)
{
	errorType err = { false,"" };
	Client* client = new Client(neighbourhood[neighbourPos]);
	string header = createHeader(height);
	cout << "JSON:" << endl << header << endl;	//DEBUG
	client->GET("/eda_coin/get_block_header/", header);
	client->sendRequest();
	clients.push_back(client);
	notifyAllObservers();
	return err;
}

errorType Node::postTransaction(unsigned int neighbourPos, Transaction tx)
{
	errorType err = { false,"" };
	Client* client = new Client(neighbourhood[neighbourPos]);
	string tx_ = createJsonTx(tx);
	cout << "JSON:" << endl << tx_ << endl;	//DEBUG
	client->POST("/eda_coin/send_tx", tx_);
	client->sendRequest();
	clients.push_back(client);
	cout << "Created Client" << endl;
	notifyAllObservers();
	return err;
}

errorType Node::postMerkleBlock(unsigned int neighbourPos)
{
	errorType err = { false,"" };
	Client* client = new Client(neighbourhood[neighbourPos]);
	//ADD CREATED CLIENTS TO CLIENT LIST
	string merkle = createJsonMerkle();
	client->POST("/eda_coin/send_merkle_block", merkle);
	cout << "JSON:" << endl << merkle << endl;	//DEBUG
	client->sendRequest();
	clients.push_back(client);
	cout << "Created Client" << endl;
	notifyAllObservers();
	return err;
}


errorType Node::postFilter(unsigned int neighbourPos)
{
	errorType err = { false,"" };
	Client* client = new Client(neighbourhood[neighbourPos]);
	string id = createJsonFilter(*myID);
	client->POST("/eda_coin/send_filter", id);
	client->sendRequest();
	clients.push_back(client);
	cout << "Created Client" << endl;
	notifyAllObservers();
	return err;
}

//JSONS
string Node::createJsonBlock(unsigned int height)
{
	json blck;
	Block block;
	for (int i = 0; i < dummieChain.size(); i++)
	{
		if (dummieChain[i].getHeight() == height)
		{
			block = dummieChain[i];
			break;
		}
	}

	auto tx = json::array();
	for (int i = 0; i < block.getNTx(); i++)
	{
		tx += json::parse(createJsonTx(block.getTransactions()[i]));
	}
	blck["tx"] = tx;
	blck["height"] = block.getHeight();
	blck["nonce"] = block.getNonce();
	blck["blockid"] = block.getBlockID();
	blck["previousblockid"] = block.getPreviousBlockID();
	blck["merkleroot"] = block.getMerkleRoot();
	blck["nTx"] = block.getNTx();

	return blck.dump();
}


string Node::createJsonTx(Transaction trans)
{
	json tx;
	tx["txid"] = trans.txId;

	tx["nTxin"] = trans.nTxIn;
	auto vin = json::array();
	for (auto i = 0; i < trans.nTxIn; i++)
	{
		vin.push_back(json::object({ {"txid",trans.vIn[i].txId}, {"blockid", trans.vIn[i].blockId} }));
	}
	tx["vin"] = vin;

	tx["nTxout"] = trans.nTxOut;
	auto vout = json::array();
	for (auto i = 0; i < trans.nTxOut; i++)
	{
		vout.push_back(json::object({ {"amount",trans.vOut[i].amount}, {"publicid", trans.vOut[i].publicId} }));
	}
	tx["vout"] = vout;

	return tx.dump();
}


string Node::createHeader(unsigned int height)
{
	Block blck;
	for (int i = 0; i < dummieChain.size(); i++)
	{
		if (dummieChain[i].getHeight() == height)
		{
			blck = dummieChain[i];
			break;
		}
	}

	string head = "Header:'block_id':" + blck.getBlockID();
	return head;
}


string Node::createJsonMerkle()
{
	json merkle;

	merkle["blockid"] = dummieChain.back().getBlockID();
	merkle["tx"] = json::parse(createJsonTx(dummieChain.back().getTransactions()[1]));
	merkle["txPos"] = 1;
	json mpath = json::array();
	MerkleTree path = dummieChain.back().getMerklePath(dummieChain.back().getTransactions()[1]);
	for (int i = 0; i < path.size(); i++)
	{
		mpath.push_back(json::object({ { "Id",path[i] } }));
	}
	merkle["merklePath"] = mpath;

	return merkle.dump();
}


string Node::createJsonFilter(string id)
{
	json filter;
	filter["Id"] = id;

	return filter.dump();
}

//SERVER
string Node::serverResponse(STATE rta)
{
	string message;

	switch (rta)
	{
	case GET:
		message = createServerOkRsp("/eda_coin/get_block_header/");
		break;

	case TX:
		message = createServerOkRsp("/eda_coin/send_tx/");
		break;


	case BLOCK:
		message = createServerOkRsp("/eda_coin/send_block/");
		break;


	case MERKLE:
		message = createServerOkRsp("/eda_coin/send_merkle_block/");
		break;


	case FILTER:
		message = createServerOkRsp("/eda_coin/sebd_filter/");
		break;

	case ERR:
		message = createServerErrRsp();
		break;
	}

	return message;

}


string Node::createServerErrRsp()
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);

	json content = { {"result",false},{"errorCode","ERROR"} };

	message += "HTTP/1.1 404 Not Found";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Cache-Control: public, max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.dump().length());
	message += CRLF;
	message += "Content-Type: text/html; charset=iso-8859-1";
	message += CRLF;
	message += CRLF;
	message += content.dump();
	message += CRLF;

	return message;
}


string Node::createServerBlock(string path)
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);

	json content;

	message += "HTTP/1.1 200 OK";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Location: 127.0.0.1" + path.substr(1);
	message += CRLF;
	message += "Cache-Control: max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.dump().length());
	message += CRLF;
	message += "Content-Type: text/html; charset=iso-8859-1";
	message += CRLF;
	message += CRLF;
	message += content.dump();
	message += CRLF;

	return message;
}

string Node::createServerOkRsp(string path)
{
	string message;
	char dateLine[100];
	char expiresLine[100];
	createDates(dateLine, expiresLine);

	json content = { {"result",true},{"errorCode",nullptr} };

	message += "HTTP/1.1 200 OK";
	message += CRLF;
	message += dateLine;
	message += CRLF;
	message += "Location: 127.0.0.1" + path.substr(1);
	message += CRLF;
	message += "Cache-Control: max-age=30";
	message += CRLF;
	message += expiresLine;
	message += CRLF;
	message += "Content-Length: ";
	message += to_string(content.dump().length());
	message += CRLF;
	message += "Content-Type: text/html; charset=iso-8859-1";
	message += CRLF;
	message += CRLF;
	message += content.dump();
	message += CRLF;

	return message;
}


void Node::createDates(char* c1, char* c2)
{
	//Fecha actual
	time_t currentTime = time(nullptr);
	struct tm t;
	struct tm* currTime = &t;
	gmtime_s(currTime, &currentTime);
	strftime(c1, 100, "Date: %a, %d %b %G %X GMT", currTime);

	//Fecha de expiracion
	struct tm t2 = t;
	struct tm* nextTime = &t2;
	if (nextTime->tm_sec >= 30) {
		if (nextTime->tm_min == 59) {
			nextTime->tm_hour++;
		}
		nextTime->tm_min = ((nextTime->tm_min) + 1) % 60;
	}
	nextTime->tm_sec = ((nextTime->tm_sec) + 30) % 60;
	strftime(c2, 100, "Expires: %a, %d %b %G %X GMT", nextTime);
}


string Node::getIP() { return *IP; }
string Node::getPort() { return to_string(port); }
ID Node::getID() { return *myID; }
const vector<sSocket>* Node::getNeighbours() { return &neighbourhood; }
const vector<Transaction>* Node::getTransactions() { return &txs; }
const vector<string>* Node::getFilters() { return &filters; }