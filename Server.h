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


typedef enum { TX, BLOCK, MERKLE, FILTER, GET, ERR } STATE;

class Server
{
public:
	Server(unsigned int port);
	void startConnection();
	void listening();
	void receiveMessage();
	void sendMessage(const string& message);
	bool getDoneListening();
	bool getDoneDownloading();
	bool getDoneSending();
	STATE getState();
	string getMessage();
	~Server();



private:

	bool doneListening;
	bool doneDownloading;
	bool doneSending;
	STATE state;
	string bodyMsg;
	string Msg;
	string response;
	char buf[10000];




	boost::system::error_code error;
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket;
	boost::asio::ip::tcp::acceptor* acceptor;
	boost::asio::io_service::work* active;
	void messaggeHandler(const boost::system::error_code err, std::size_t bytes);
	void connectionHandler(const boost::system::error_code& err);
	void writingHandler(const boost::system::error_code err, std::size_t bytes);
	STATE parseMessage();
	bool validateBlock(string blck);
	bool validateTx(string tx);
	bool validateFilter(string filter);
};