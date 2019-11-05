#pragma once
#include "observer.h"
#include "imgui.h"
#include "EDANet.h"

class NetViewer :
	public Observer
{
public:
	NetViewer();
	virtual void update(void*);
	void draw();
private:
	unsigned int nodeAmount;
};

