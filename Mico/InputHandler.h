#pragma once
#include <vector>
#include "Observer.h"
#include <GLFW\glfw3.h>

class InputHandler
{
public:
	
	~InputHandler();
	void addObserver(Observer* obs);
	static InputHandler* GetInstance();
	void notify(Event evt);

private:
	InputHandler();
	std::vector<Observer*> observers;
	static InputHandler* instance;
};

