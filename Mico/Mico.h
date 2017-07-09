#pragma once
#include <map>
#include "System.h"
#include "TexturedModel.h"
#include "GraphicsSystem.h"
#include "WindowSystem.h"
#include "WindowHandler.h"
#include "Loader.h"
#include "GlobalLight.h"
#include "Terrain.h"
#include "Material.h"
#include "EntityManager.h"
#include "UISystem.h"

using namespace std;
class Mico
{
public:
	static Mico* GetInstance();
	void Initialize();
	void Run();

	~Mico();

private:
	
	Mico();
	static Mico* Instance;
	map<string, System*> Systems;
	bool running;
	double previous;
	double steps;
	WindowSystem* windowSystem;
	GraphicsSystem* graphicsSystem;
	UISystem* uiSystem;
	
};

