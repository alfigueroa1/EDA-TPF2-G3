#include "EDANet.h"

#include <iostream>
using namespace std;

EDANet::EDANet() {

}
EDANet::~EDANet() {

}

bool EDANet::createNode(const string& IP, const string& port, const string& ID)	//ALEX
{
	bool ret(true);

	cout << "IP: " << IP << endl;
	cout << "Port:" << port << endl;
	cout << "ID: " << ID << endl;

	nodes.emplace_back(ID);
	

	notifyAllObservers();

	return ret;
}

Node* EDANet::getNode(unsigned int nodePos) //la hice por el mero hecho de poder probar mis clases
{
	Node* rta = nullptr;
	if (nodePos < nodes.size() && nodePos >= 0)
		rta = &nodes[nodePos];
	return rta;
}
