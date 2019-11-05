#include "EDANet.h"

#include <iostream>
using namespace std;

EDANet::EDANet() {

}
EDANet::~EDANet() {
	for (int i = 0; i < nodes.size(); i++)
		delete nodes[i];
}

errorType EDANet::createNode(const string& IP, const string& port, const string& ID)	//ALEX
{
	errorType err;
	err.error = false;
	err.datos = "";

	if (existAlready(ID) == false ) {

		cout << "IP: " << IP << endl;
		cout << "Port:" << port << endl;
		cout << "ID: " << ID << endl;

		Node* newNode = new Node(ID, port, IP);

		nodes.emplace_back(newNode);

		notifyAllObservers();
	}
	else {
		err.error = true;
		err.datos = "YA EXISTE UN NODO CON ESA ID";
	}

	return err;
}

Node* EDANet::getNode(unsigned int nodePos) //la hice por el mero hecho de poder probar mis clases
{
	Node* rta = nullptr;
	if (nodePos < nodes.size() && nodePos >= 0)
		rta = nodes[nodePos];
	return rta;
}

bool EDANet::existAlready(const string& _id)
{
	bool rta = false;

	for (int i = 0; (rta == false) && (i < nodes.size()); i++) {
		if (_id == nodes[i]->getID())
			rta = true;
	}

	return rta;
}
