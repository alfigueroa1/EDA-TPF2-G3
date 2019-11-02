#pragma once

#include "subject.h"
#include "Node.h"
#include "blockChain.h"

class EDANet : public Subject {
public:
	EDANet();
	~EDANet();
	bool createNode(const string& IP, const string& port, const string& ID);	//ALTERADO POR ALEX!!!
	unsigned int getNodeAmount(void) { return nodes.size(); };	//OJITO CON LOS PARENTESIS

private:
	vector<Node> nodes;
};