/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "server.h"
#include "json.hpp"

using json = nlohmann::json;

#define CRLF "\x0D\x0A"

/*******************************************************************************
 * CLASS METHODS DEFINITIONS
 ******************************************************************************/
Server::Server(unsigned int port)
{
	doneDownloading = false;
	doneListening = false;
	doneSending = false;
	receiveMsg.clear();

	IO_handler = new boost::asio::io_service();
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), port);
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler, ep);
}

Server::~Server()
{
	acceptor->close();
	socket->close();
	delete acceptor;
	delete socket;
	delete IO_handler;
}


void Server::startConnection() 
{
	doneListening = false;
	doneSending = false;
	acceptor->non_blocking(true);
}

void Server::listening() 
{
	acceptor->accept(*socket, error);
	if (error.value() != WSAEWOULDBLOCK)
	{
		doneListening = true;
		cout << "Connected" << endl;
	}
}


void Server::receiveMessage()
{
	char buf[1000] = {};
	size_t len = 0;
	len = socket->read_some(boost::asio::buffer(buf), error);
	if (error.value() != WSAEWOULDBLOCK)
	{
		doneDownloading = true;
		cout << buf << endl;
	}
	receiveMsg += buf;
}



STATE Server::parseMessage()
{
	STATE rta = ERR;

	if (receiveMsg.find("GET") != string::npos)
	{
		size_t pos = receiveMsg.find_last_of(CRLF, receiveMsg.length() - strlen(CRLF));
		string json = receiveMsg.substr(pos + 1);

		if (receiveMsg.find("/eda_coin/get_block_header/"))
		{
			rta = GET;
		}

		else
		{
			rta = ERR;
		}
	}

	else if (receiveMsg.find("POST") != string::npos)
	{
		size_t pos = receiveMsg.find_last_of(CRLF, receiveMsg.length() - strlen(CRLF));
		string json = receiveMsg.substr(pos + 1);

		if (receiveMsg.find("/eda_coin/send_block") != string::npos)
		{
			if (validateBlock(json))
			{
				rta = BLOCK;
			}
		}

		else if (receiveMsg.find("/eda_coin/send_tx") != string::npos)
		{
			if (validateTx(json))
			{
				rta = TX;
			}
		}

		else if (receiveMsg.find("/eda_coin/send_merkle_block") != string::npos)
		{
			//Not yet
			rta = MERKLE;
		}

		else if (receiveMsg.find("/eda_coin/send_filter") != string::npos)
		{
			if (validateFilter(json))
			{
				rta = FILTER;
			}
		}

		else
		{
			rta = ERR;
		}

	}

	else
	{
		rta = ERR;
	}

	return rta;
}


void Server::sendMessage(const string& message)
{
	size_t len;
	len = socket->write_some(boost::asio::buffer(message, strlen(message.c_str())), error);

	if (error.value() != WSAEWOULDBLOCK)
	{
		doneSending = true;
		cout << message << endl;
	}
	
}


bool Server::getDoneListening() { return doneListening; }
bool Server::getDoneSending() { return doneSending; }
bool Server::getDoneDownloading() { return doneDownloading; }


bool Server::validateBlock(string blck)
{
	bool ret = false;

	try
	{
		json block = json::parse(blck);

		//Block
		if (block.size() == 7) //Si son 7 elementos
		{
			block.at("height");
			block.at("nonce");
			block.at("blockid");	//Se fija que sean los correspondientes
			block.at("previousblockid");
			block.at("merkleroot");
			int ntx = block.at("nTx");
			block.at("height");
			block.at("tx");

			//Transactions
			auto arrayTrans = block["tx"];
			for (auto& trans : arrayTrans)	//Parsea todas las transacciones
			{
				if (arrayTrans.size() == ntx && trans.size() == 5)	//Si son 5 elementos
				{
					trans.at("txid");
					int txin = trans.at("nTxin");
					trans.at("vin");	//Se fija que sean los correctos
					int txout = trans.at("nTxout");
					trans.at("vout");

					auto vIn = trans["vin"];
					auto vOut = trans["vout"];
					if (vIn.size() == txin && vOut.size() == txout)
					{
						for (auto& elsi : vIn)
						{
							elsi.at("blockid");
							elsi.at("txid");
						}

						for (auto& elso : vOut)
						{
							elso.at("publicid");
							elso.at("amount");
						}

						ret = true;
					}
				}
			}
		}
	}

	catch (std::exception& e)
	{
		ret = false;
	}

	return ret;
}



bool Server::validateTx(string tx)
{
	bool ret = false;

	try
	{
		json trans = json::parse(tx);

		if (trans.size() == 5)	//Si son 5 elementos
		{
			trans.at("txid");
			int txin = trans.at("nTxin");
			trans.at("vin");	//Se fija que sean los correctos
			int txout = trans.at("nTxout");
			trans.at("vout");

			auto vIn = trans["vin"];
			auto vOut = trans["vout"];
			if (vIn.size() == txin && vOut.size() == txout)
			{
				for (auto& elsi : vIn)
				{
					elsi.at("blockid");
					elsi.at("txid");
				}

				for (auto& elso : vOut)
				{
					elso.at("publicid");
					elso.at("amount");
				}

				ret = true;
			}
		}
	}

	catch (std::exception& e)
	{
		ret = false;
	}

	return ret;
}


bool Server::validateFilter(string filter)
{
	bool ret = false;

	try
	{
		json fltr = json::parse(filter);
		fltr.at("Id");
		ret = true;
	}

	catch (std::exception& e)
	{
		ret = false;
	}

	return ret;
}