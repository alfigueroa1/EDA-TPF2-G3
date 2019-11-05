#pragma once

#include "subject.h"
#include "Node.h"
#include "blockChain.h"

class EDANet : public Subject {
public:
	EDANet();
	~EDANet() { for (int i = 0; i < nodes.size(); i++) delete nodes[i]; }
	bool createNode(const string& IP, const string& port, const string& ID) {
		Node* newNode = new Node(ID, port, IP);
		nodes.emplace_back(newNode);
		notifyAllObservers();
		return true;
	}
	unsigned int getNodeAmount(void) { return nodes.size(); };
	Node* getNode(unsigned int nodePos) { Node* rta = nullptr; if (nodePos < nodes.size() && nodePos >= 0) rta = nodes[nodePos]; return rta; }

private:
	vector<Node *> nodes;
};