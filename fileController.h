pragma once

#include "observer.h"
#include "model.h"

class fileController : public Observer {
public:
	fileController(Model&);
	virtual void update(void*);
	string askPath();

private:
	Model* m;
	char path[_MAX_PATH];
	vector<string> filenames;
};
