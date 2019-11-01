#pragma once
#include "observer.h"
#include "model.h"

class blockController : public Observer {
public:
	blockController(Model&);
	virtual void update(void*);
	void askBlock(string);
	void selectBlock(string);

private:
	Model* m;
	int currBlock;
	string blocks;
	//void openBlock(unsigned long int);
};