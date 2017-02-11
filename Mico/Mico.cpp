#include "Mico.h"
#include "GraphicsSystem.h"
#include "WindowSystem.h"
#include "WindowHandler.h"

Mico::Mico()
{
}


Mico * Mico::GetInstance()
{
	if (!Instance)
	{
		Instance = new Mico();
	}
	return Instance;	
}

void Mico::Initialize()
{
	WindowSystem* windowSystem = new WindowSystem(800,600);
	GraphicsSystem* graphicsSystem = new GraphicsSystem(windowSystem->Getsize());
	WindowHandler::GetInstance()->addObserver(graphicsSystem);
}

void Mico::Run()
{
}

Mico::~Mico()
{
}
