#pragma once
#include "observer.h"
#include "Node.h"
class NodeController :
	public Observer
{
public:
	NodeController(Node& model);
	virtual void update(void*);
	void cycle();
};

