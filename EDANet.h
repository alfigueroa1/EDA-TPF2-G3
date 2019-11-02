#pragma once

#include "subject.h"
#include "Node.h"
#include "blockChain.h"

class EDANet : public Subject {
public:
	EDANet();
	~EDANet();
	bool createNode(string IP, string port);
	unsigned int getNodeAmount(void) { return nodes.size; };

private:
	vector<Node> nodes;
};