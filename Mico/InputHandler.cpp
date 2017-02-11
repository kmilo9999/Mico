#include "InputHandler.h"
#include <iostream>

InputHandler* InputHandler::instance(0);

InputHandler::InputHandler()
{
	
}

InputHandler::~InputHandler()
{
}

void InputHandler::addObserver(Observer * obs)
{
	observers.push_back(obs);
}

InputHandler* InputHandler::GetInstance()
{
	if (!instance)
	{
		instance = new InputHandler();
	}
	return instance;
}

void InputHandler::notify(Event evt)
{
	for (Observer* obs : observers)
	{
		obs->onNotify(evt);
	}
}

