#include "Mico.h"
#include <vector>
#include <assert.h>

Mico* Mico::Instance(0);

double secsPerFrame = 1 / 60;




Mico::Mico():running(true),previous(0.0),steps(0.0)
{
	windowSystem = new WindowSystem(800, 600);
	graphicsSystem = new GraphicsSystem( windowSystem->Getsize());
	uiSystem = new UISystem();
}


Mico::~Mico()
{
	lua_close(L);
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
	
	windowSystem->Init();

	graphicsSystem->Init();
	uiSystem->Init();

	WindowHandler::GetInstance()->addObserver(graphicsSystem);
	EntityManager::GetInstance()->Initialize();
	graphicsSystem->InitScene();
	uiSystem->addObserver(graphicsSystem);
	uiSystem->addObserver(EntityManager::GetInstance());
	
   //Lua (For now, it must be changed to some kind of system or components)
   // Init Lua
	L = luaL_newstate();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);
	luaopen_debug(L);


	luaL_dofile(L, "scripts/middle_entity.lua");
	
}


/*
	Run the Game Loop where updates all the componentes of the scene
*/
void Mico::Run()
{
	
	glfwSwapInterval(1);
	previous = glfwGetTime();
	double lag = 0;
	while (running)
	{
		double current = glfwGetTime();
		double elapsed = current - previous;
		previous = current;
		steps += elapsed;

		EntityManager::GetInstance()->UpdateEntities();

		graphicsSystem->Update();

		uiSystem->ImGuiNewFrame();
		uiSystem->Update();

		graphicsSystem->FinishRender();
		//while (lag >= secsPerFrame)
		//{
		//	lag -= secsPerFrame;
		//}

		//TODO Sent entities! 
		if (glfwWindowShouldClose(windowSystem->GetWindow()))
		{
			running = false;
		}
		
	}

	uiSystem->ImGuiShutdown();
	
}


