#include "Node.h"

Node::Node(string _ID, string _port, string _IP) {

}
Node::~Node() {

}
errorType Node::postBlock(unsigned int neighbourPos) {
	errorType error;

	return error;
}
errorType Node::getBlockHeader(unsigned int height, unsigned int neighbourPos) {
	errorType error;

	return error;
}
errorType Node::postTransaction(unsigned int neighbourPos, Transaction tx) {
	errorType error;

	return error;
}
errorType Node::postMerkleBlock(unsigned int neighbourPos) {
	errorType error;

	return error;
}
errorType Node::postFilter(unsigned int neighbourPos) {
	errorType error;

	return error;
}

errorType Node::AddNeighbour(const string& _ID, const string& _port)
{
	errorType error;
	return error;
}

ID Node::getID() {
	ID aux;

	return aux;
}
void keepListening() {

}
void keepSending() {

}
void Node::addBlock(Block block) {

}