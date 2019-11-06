/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "server.h"
#include "json.hpp"

using json = nlohmann::json;


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
		if(receiveMsg.find("/eda_coin/get_block_header/"))
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
		if (receiveMsg.find("/eda_coin/send_block"))
		{
			rta = POST;
		}

		else if (receiveMsg.find("/eda_coin/send_tx"))
		{
			rta = POST;
		}

		else if (receiveMsg.find("/eda_coin/send_merkle_block"))
		{
			rta = POST;
		}

		else if (receiveMsg.find("/eda_coin/send_filter"))
		{
			rta = POST;
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
	do {
		len = socket->write_some(boost::asio::buffer(message, strlen(message.c_str())), error);

	} while (error.value() == WSAEWOULDBLOCK);

	if (error.value() != WSAEWOULDBLOCK)
	{
		doneSending = true;
		cout << message << endl;
	}
	
}
bool Server::getDoneListening() { return doneListening; }
bool Server::getDoneSending() { return doneSending; }
bool Server::getDoneDownloading() { return getDoneDownloading; }