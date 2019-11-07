#include "EDANet.h"

EDANet::EDANet() {

}
EDANet::~EDANet() {
	for (int i = 0; i < nodes.size(); i++) delete nodes[i];
}

errorType EDANet::createNode(const string& IP, const string& port, const string& ID) {
	errorType err;
	err.error = false;
	err.datos = "";

	if (existAlready(ID) == false) {

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