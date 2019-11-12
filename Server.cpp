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
	bodyMsg.clear();
	Msg.clear();
	*buf = {};


	IO_handler = new boost::asio::io_service();
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), port);
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler, ep);
	active = new boost::asio::io_service::work(*IO_handler);
}

Server::~Server()
{
	acceptor->close();
	socket->close();
	delete acceptor;
	delete socket;
	delete active;
	delete IO_handler;
}


void Server::startConnection() 
{
	doneListening = false;
	doneSending = false;
	acceptor->non_blocking(true);
	IO_handler->poll();
}

void Server::listening() 
{
	IO_handler->poll();
	acceptor->async_accept(*socket, boost::bind(&Server::connectionHandler,this, boost::asio::placeholders::error));
}

void Server::connectionHandler(const boost::system::error_code& err)
{
	if (!err)
	{
		doneListening = true;
		cout << "Connected" << endl;
	}

	else 
	{
		cout << err.message() << endl;
		cout << "Error while listening" << endl;
	}
}


void Server::receiveMessage()
{
	IO_handler->poll();
	socket->async_read_some(boost::asio::buffer(buf), boost::bind(&Server::messaggeHandler, this, 
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


void Server::messaggeHandler(const boost::system::error_code err, std::size_t bytes)
{
	string aux = buf;
	Msg += buf;

	if(!err && (aux.find("Expect") != string::npos))
	{
		doneDownloading = false;
		cout << "Still Downoading" << endl;
	}

	else if (!err)
	{
		doneDownloading = true;
		cout << "Finished Download" << endl;
		state = parseMessage();
		cout << Msg << endl;
	}


	else
	{
		cout << "Error while receiving" << endl;
	}

}

STATE Server::getState() {
	return state;
}

STATE Server::parseMessage()
{
	STATE rta = ERR;

	if (Msg.find("GET") != string::npos)
	{
		if (Msg.find("/eda_coin/get_block_header/"))
		{
			rta = GET;
		}

		else
		{
			rta = ERR;
		}
	}

	else if (Msg.find("POST") != string::npos)
	{
		size_t pos = Msg.find_last_of(CRLF, Msg.length() - strlen(CRLF));
		bodyMsg = Msg.substr(pos + 1);

		if (Msg.find("/eda_coin/send_block") != string::npos)
		{
			if (validateBlock(bodyMsg))
			{
				rta = BLOCK;
			}
		}

		else if (Msg.find("/eda_coin/send_tx") != string::npos)
		{
			if (validateTx(bodyMsg))
			{
				rta = TX;
			}
		}

		else if (Msg.find("/eda_coin/send_merkle_block") != string::npos)
		{
			//Not yet
			rta = MERKLE;
		}

		else if (Msg.find("/eda_coin/send_filter") != string::npos)
		{
			if (validateFilter(bodyMsg))
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
	response = message;
	IO_handler->poll();
	socket->async_write_some(boost::asio::buffer(response, strlen(response.c_str())), boost::bind(&Server::writingHandler,this,
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


void Server::writingHandler(const boost::system::error_code err, std::size_t bytes)
{
	if (!err)
	{
		doneSending = true;
		cout << response << endl;
	}

	else
	{
		cout << "Error while sending" << endl;
	}
}

bool Server::getDoneListening() { return doneListening; }
bool Server::getDoneSending() { return doneSending; }
bool Server::getDoneDownloading() { return doneDownloading; }
string Server::getMessage() { return bodyMsg; }


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