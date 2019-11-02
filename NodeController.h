#pragma once
#include "observer.h"
#include "Node.h"

enum controlState { OUT, PBLOCK, GBHEADER, PTX, PMBLOCK, PFILTER, ADDN };

class NodeController :
	public Observer
{
public:
	NodeController(Node& model);
	virtual void update(void*);
	void cycle();
private:
	Node* model;

	string mID;
	string windowName;
	vector<sSocket> mNeighbours;

	controlState cstate;
	void drawOut();
	void drawPBlock();
	void drawGBHeader();
	void drawPTX();
	void drawPMBlock();
	void drawPFilter();
	void drawAddNode();

	void returnButton();

};

