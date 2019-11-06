#pragma once
#include "sSocket.h"
#include <iostream>
#include <string>
#include <curl.h>

using namespace std;

class Client 
{
public:
	Client(sSocket socket);
	~Client();

	void sendRequest(void);
	void POST(string path, string json);
	void GET(string path, string json);
	string getResponse(void);
	int getRunning();
	

private:

	CURLM* curlm;
	CURL* curl;
	string host;
	string response;
	int running;
};