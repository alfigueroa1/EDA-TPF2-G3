/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Node.h"
#include "json.hpp"
#include <iostream>

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

	cout << "Created (the good way)" << endl;
}

//DESTRUCTOR
Node::~Node() { // igual el destructor

	cout << "Destroyed" << endl;

	delete myID;
	delete IP;

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

void keepListening() {

}
void keepSending() {

}
void Node::addBlock(Block block) {

}

//POST-GET
errorType Node::postBlock(unsigned int neighbourPos, unsigned int height)
{
	errorType err = { false,"" };
	Client client(neighbourhood[neighbourPos]);
	string blck = createJsonBlock(height);

	client.POST("/eda_coin/send_block", blck);
	client.sendRequest();

	notifyAllObservers();
	return err;
}

errorType Node::getBlockHeader(unsigned int height, unsigned int neighbourPos)
{
	errorType err = { false,"" };
	Client client(neighbourhood[neighbourPos]);
	string header = createHeader(height);

	client.GET("/eda_coin/get_block_header/", header);
	client.sendRequest();

	notifyAllObservers();
	return err;
}

errorType Node::postTransaction(unsigned int neighbourPos, Transaction tx)
{
	errorType err = { false,"" };
	Client client(neighbourhood[neighbourPos]);
	string tx_ = createJsonTx(tx);

	client.POST("/eda_coin/send_tx", tx_);
	client.sendRequest();

	notifyAllObservers();
	return err;
}

errorType Node::postMerkleBlock(unsigned int neighbourPos)
{
	errorType err = { false,"" };
	Client client(neighbourhood[neighbourPos]);
	string merkle = createJsonMerkle();
	client.POST("/eda_coin/send_merkle_block", merkle);

	client.sendRequest();

	notifyAllObservers();
	return err;
}


errorType Node::postFilter(unsigned int neighbourPos)
{
	errorType err = { false,"" };
	Client client(neighbourhood[neighbourPos]);
	string id = createJsonFilter(*myID);
	client.POST("/eda_coin/send_filter", id);

	client.sendRequest();

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
	blck["heigh"] = block.getHeight();
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
		vin.push_back(json::object({ {"amount",trans.vOut[i].amount}, {"publicid", trans.vOut[i].publicId} }));
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
	merkle["txPos"] = 4;
	json mpath = json::array();
	MerkleTree path = dummieChain.back().getMerklePath(dummieChain.back().getTransactions()[1]);
	for (int i = 0; i < path.size(); i++)
	{
		mpath.push_back({ "Id",path[i] });
	}
	merkle["merklePath"] = mpath;

	return merkle.dump();
}


string Node::createJsonFilter(string id)
{
	json filter;
	filter["id"] = id;

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

/*Node::Node(Node& n)
{
	cout << "Copied" << endl;
	//todo
}*/
