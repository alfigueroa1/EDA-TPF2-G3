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
	errorType error(false);

	cout << "Block posted to neighbour" << neighbourPos <<endl;

	return error;
}
errorType Node::getBlockHeader(unsigned int height, unsigned int neighbourPos) {
	errorType error(false);

	cout << "Block header number " << height << "asked to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::postTransaction(unsigned int neighbourPos, Transaction tx) {
	errorType error(false);

	cout << "Tx posted to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::postMerkleBlock(unsigned int neighbourPos) {
	errorType error(false);

	cout << "Merkle Block posted to neighbour " << neighbourPos << endl;

	return error;
}
errorType Node::postFilter(unsigned int neighbourPos) {
	errorType error(false);

	cout << "Filter posted to neighbour " << neighbourPos << endl;

	return error;
}
void Node::AddNeighbour(const string& _IP,const string& _port) {	//ALEX!!!	//punteros
	neighbourhood->emplace_back(_IP,_port);
	notifyAllObservers();
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
