#pragma once
#include "Event.h"
#include "Observer.h"
#include <vector>
class Observable
{
public:
	virtual  ~Observable() {};
	virtual void addObserver(Observer* obs) 
	{
		observers.push_back(obs);
	}

	virtual void notify(Event& event) 
	{
		for (Observer* obs : observers)
		{
			obs->onNotify(event);
		}
	};
protected:
	std::vector<Observer*> observers;
};

