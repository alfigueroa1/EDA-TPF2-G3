#pragma once
#include "observer.h"
class NodeViewer :
	public Observer
{
public:
	NodeViewer();
	virtual void update(void*);
	void cycle();
};

