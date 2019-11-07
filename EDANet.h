#pragma once

#include "subject.h"
#include "Node.h"
#include "blockChain.h"

class EDANet : public Subject {
public:
	EDANet();
	~EDANet();
	errorType createNode(const string& IP, const string& port, const string& ID) {};
	unsigned int getNodeAmount(void) { return nodes.size(); };
	Node* getNode(unsigned int nodePos) { Node* rta = nullptr; if (nodePos < nodes.size() && nodePos >= 0) rta = nodes[nodePos]; return rta; }

private:
	vector<Node *> nodes;
	bool existAlready(const string& _id) {

		bool rta = false;

		for (int i = 0; (rta == false) && (i < nodes.size()); i++) {
			if (_id == nodes[i]->getID())
				rta = true;
		}

		return rta;
	}
};