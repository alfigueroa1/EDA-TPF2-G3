#pragma once
#include "observer.h"
#include "Node.h"

class NodeViewer :
	public Observer
{
public:
	NodeViewer();
	virtual void update(void*);
	void cycle();
private:
	string id;
	string port;
	string ip;
	string windowName;
	vector<sSocket> neigbours;
	vector<Transaction> tx;
	vector<string> filters;

	void showData();
	void showNeighbours();
	void showTx();
	void showFilters();
};