#include "Node.h"

Node::Node(string _ID) : myID(_ID) {

}
Node::~Node() {

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
void Node::AddNeighbour(string _ID, string _port) {

}

ID Node::getID() {			//ALEX
	return myID;
}
void keepListening() {

}
void keepSending() {

}
void Node::addBlock(Block block) {

}