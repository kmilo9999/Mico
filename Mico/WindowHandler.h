#pragma once
#include "Observer.h"
#include <vector>

class WindowHandler
{
public:
	
	static WindowHandler* GetInstance();
	void addObserver(Observer* obs);
	void notify(Event evt);
	~WindowHandler();

private:
	WindowHandler();
	std::vector<Observer*> observers;
	static WindowHandler* instance;

};

