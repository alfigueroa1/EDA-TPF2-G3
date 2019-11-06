#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace std;


typedef enum { POST, GET, ERR } STATE;

class Server
{
public:
	Server(unsigned int port);
	void startConnection();
	void listening();
	void receiveMessage();
	STATE parseMessage();
	void sendMessage(const string& message);
	bool getDoneListening();
	bool getDoneDownloading();
	bool getDoneSending();
	~Server();



private:

	bool doneListening;
	bool doneDownloading;
	bool doneSending;
	string receiveMsg;
	boost::system::error_code error;
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket;
	boost::asio::ip::tcp::acceptor* acceptor;
};