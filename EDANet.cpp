#include "EDANet.h"

EDANet::EDANet() {

}
EDANet::~EDANet() {
	for (int i = 0; i < nodes.size(); i++) delete nodes[i];
}

//bool EDANet::createNode(string IP, string port) {
//	bool ret;
//
//
//	return ret;
//}