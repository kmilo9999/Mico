#include "InputHandler.h"
#include <iostream>

InputHandler* InputHandler::instance(0);

InputHandler::InputHandler()
{
	
}

InputHandler::~InputHandler()
{
}

InputHandler* InputHandler::GetInstance()
{
	if (!instance)
	{
		instance = new InputHandler();
	}
	return instance;
}



