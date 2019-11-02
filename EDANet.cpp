#include "EDANet.h"

#include <iostream>
using namespace std;

EDANet::EDANet() {

}
EDANet::~EDANet() {

}

bool EDANet::createNode(const string& IP, const string& port, const string& ID)
{
	bool ret(true);

	cout << "IP: " << IP << endl;
	cout << "Port:" << port << endl;
	cout << "ID: " << ID << endl;

	nodes.emplace_back();

	notifyAllObservers();

	return ret;
}