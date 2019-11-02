#pragma once
#include "observer.h"
class NetViewer :
	public Observer
{
public:
	virtual void update(void*);
	void cycle();
};

