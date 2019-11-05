#include "Node.h"

#include <iostream>
using namespace std;

Node::Node(string _ID, string _port, string _IP) { //constructor modificado para trabajar con punteros
	port = stoi(_port);
	myID = new string(40,' ');
	IP = new string(40,' ');
	*myID = _ID;
	*IP = _IP;

	neighbourhood = new vector<sSocket>; // no se q demonios hacer

	cout << "Created (the good way)" << endl;
}
Node::~Node() { // igual el destructor

	cout << "Destroyed" << endl;

	delete myID;
	delete IP;
	
	delete neighbourhood;
}
errorType Node::postBlock(unsigned int neighbourPos) {
	errorType error;
	error.error = false;
	error.datos = "";

	cout << "Block posted to neighbour" << neighbourPos <<endl;

	if (neighbourPos == 2) {
		error.error = true;
		error.datos = "No, mira, te explico. Yo no me llevo bien con ese, entendes?";
	}

	return error;
}
errorType Node::getBlockHeader(unsigned int height, unsigned int neighbourPos) {
	errorType error;
	error.error = false;
	error.datos = "";

	if (height > 20) {
		error.error = true;
		error.datos = "No papa, te pasaste. Mi block chain solo tiene 20 bloques, y me pediste cualquiera. Jaj";
	}

	cout << "Block header number " << height << "asked to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::postTransaction(unsigned int neighbourPos, Transaction tx) {
	errorType error;
	error.error = false;
	error.datos = "";

	cout << "Tx posted to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::postMerkleBlock(unsigned int neighbourPos) {
	errorType error;
	error.error = false;
	error.datos = "";

	cout << "Merkle Block posted to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::postFilter(unsigned int neighbourPos) {
	errorType error;
	error.error = false;
	error.datos = "";

	cout << "Filter posted to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::AddNeighbour(const string& _IP,const string& _port) {	//ALEX!!!	//punteros
	neighbourhood->emplace_back(_IP,_port);
	notifyAllObservers();

	errorType error;
	error.error = false;
	error.datos = "";

	if (stoi(_port) < 0) {
		error.error = true;
		error.datos = "No querido, que clase de puerto es ese? Te fumaste algo fuerte?";
	}

	return error;
}

ID Node::getID() {			//ALEX //punteros
	return *myID;
}
void keepListening() {

}
void keepSending() {

}
void Node::addBlock(Block block) {

}

/*Node::Node(Node& n)
{
	cout << "Copied" << endl;
	//todo
}*/
