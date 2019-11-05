#pragma once

#include "subject.h"
#include "Node.h"
#include "blockChain.h"

class EDANet : public Subject {
public:
	EDANet();
	~EDANet();
	errorType createNode(const string& IP, const string& port, const string& ID) {
		errorType err;
		err.error = false;
		err.datos = "";

		if (existAlready(ID) == false) {

			Node* newNode = new Node(ID, port, IP);
			nodes.emplace_back(newNode);
			notifyAllObservers();
		}
		else {
			err.error = true;
			err.datos = "YA EXISTE UN NODO CON ESA ID";
		}
		return err;
	}
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