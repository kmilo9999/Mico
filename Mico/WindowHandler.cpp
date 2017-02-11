#include "WindowHandler.h"


WindowHandler* WindowHandler::instance(0);

WindowHandler::WindowHandler()
{
}

WindowHandler::~WindowHandler()
{

}

WindowHandler * WindowHandler::GetInstance()
{
	if (!instance)
	{
		instance = new WindowHandler();
	}
	return instance;
}

void WindowHandler::addObserver(Observer * obs)
{
	observers.push_back(obs);
}

void WindowHandler::notify(Event evt)
{
	for (Observer* obs : observers)
	{
		obs->onNotify(evt);
	}
}

