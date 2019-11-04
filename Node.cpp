#include "Node.h"

Node::Node(string _ID, string _port, string _IP) { //constructor modificado para trabajar con punteros
	port = stoi(_port);
	myID = new string(40,' ');
	IP = new string(40,' ');
	*myID = _ID;
	*IP = _IP;

	neighbourhood = new vector<sSocket>; // no se q demonios hacer
}
Node::~Node() { // igual el destructor
	delete myID;
	delete IP;
}
errorType Node::postBlock(unsigned int neighbourPos) {
	errorType error(false);

	return error;
}
errorType Node::getBlockHeader(unsigned int height, unsigned int neighbourPos) {
	errorType error(false);

	return error;
}
errorType Node::postTransaction(unsigned int neighbourPos, Transaction tx) {
	errorType error(false);

	return error;
}
errorType Node::postMerkleBlock(unsigned int neighbourPos) {
	errorType error(false);

	return error;
}
errorType Node::postFilter(unsigned int neighbourPos) {
	errorType error(false);

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