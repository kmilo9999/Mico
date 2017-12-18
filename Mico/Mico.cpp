#include "Mico.h"
#include <vector>
#include <assert.h>
#include <algorithm>

Mico* Mico::Instance(0);

const float secsPerFrame = 1000/60;
const float maxSteps = 1.0f;




Mico::Mico():running(true),previous(0.0),steps(0.0)
{
	windowSystem = new WindowSystem(800, 600);
	graphicsSystem = new GraphicsSystem( windowSystem->Getsize());
	uiSystem = new UISystem();
}


Mico::~Mico()
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
	
	windowSystem->Init();

	graphicsSystem->Init();
	uiSystem->Init();
	//scriptSystem->Init();

	WindowHandler::GetInstance()->addObserver(graphicsSystem);
	EntityManager::GetInstance()->Initialize();
	graphicsSystem->InitScene();
	uiSystem->addObserver(graphicsSystem);
	uiSystem->addObserver(EntityManager::GetInstance());
	
}


/*
	Run the Game Loop where updates all the componentes of the scene
*/
void Mico::Run()
{
	
	glfwSwapInterval(1);
	previous = glfwGetTime();
	double lag = 0;
	double previous = glfwGetTime();
	while (running)
	{
		float newTicks = glfwGetTime();
		float elapsed = newTicks - previous;
		previous = newTicks;
		float deltaTime = elapsed / secsPerFrame;



		
		while (deltaTime > 0.0f){	
			float dt = std::min(deltaTime, maxSteps);
			EntityManager::GetInstance()->UpdateCenterModel(deltaTime);
			deltaTime -= dt;
		
		}

		EntityManager::GetInstance()->UpdateEntities();
		
	

		graphicsSystem->Update(elapsed);

		uiSystem->ImGuiNewFrame();
		uiSystem->Update(elapsed);

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


