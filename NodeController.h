#pragma once
#include "observer.h"
#include "Node.h"

#define COMBO_SIZE	50
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
	bool availableNeighbours;
	int currNeighbour;
	char comboNeighbour[COMBO_SIZE + 1];

	controlState cstate;
	void drawOut();
	void drawPBlock();
	void drawGBHeader();
	void drawPTX();
	void drawPMBlock();
	void drawPFilter();
	void drawAddNode();

	void returnButton();
	void neighbourSelect();

	static bool comboGetter(void* data, int idx, const char** out_str);


};

