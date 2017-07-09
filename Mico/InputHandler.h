#pragma once
#include <vector>

#include <GLFW\glfw3.h>
#include "Observable.h"

class InputHandler: public Observable
{
public:
	
	~InputHandler();
	static InputHandler* GetInstance();

private:
	InputHandler();
	static InputHandler* instance;
};

