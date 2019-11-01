#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <vector>
#include "observer.h"

using namespace std;


/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/
class Subject {
public:
	void attach(Observer& o) { observers.push_back(&o); };
	void detach(vector<Observer*>::iterator pos) { observers.erase(pos); };
protected:
	void notifyAllObservers(void) {
		for (Observer* o : observers)
			o->update(this);
	}
	vector<Observer*> observers;
};