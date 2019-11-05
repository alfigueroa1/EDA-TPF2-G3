#pragma once

#include "observer.h"
#include "EDANet.h"
#include "NetViewer.h"

#include "NodeController.h"
#include "NodeViewer.h"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include "WarningWindowHandler.h"

class NetController :
	public Observer
{
public:
	NetController(EDANet& model);
	~NetController();
	virtual void update(void*); //aqui crea nuevos modelos y controllers
	bool isRunning();
	void cycle();
private:
	EDANet* model;
	NetViewer viewer;
	bool running;

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ImVec4 backColor;

	void init_interface();
	void close_interface();
	void start_frame();
	void end_frame();

	void controlWindow();
	char IDbuf[50];
	int IParr[4];
	int Port;

	vector<NodeController*> controllerlist;
	vector<NodeViewer*> viewerlist;

	WarningWindowHandler whandler;
};

