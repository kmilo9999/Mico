#include "Mico.h"
#include <vector>


Mico* Mico::Instance(0);

double secsPerFrame = 1 / 60;


vector<vec3> vertices{  vec3(0.5f,  0.5f, 0.0f),
						vec3(0.5f, -0.5f, 0.0f),
						vec3(-0.5f, -0.5f, 0.0f),
						vec3(-0.5f,  0.5f, 0.0f) };

vector<int> indexes{ 2,1,0,
					  0,3,2 };

vector<vec2> textcoor{
	vec2(1.0f, 1.0f), // Top Right
	vec2(1.0f, 0.0f), // Bottom Right
	vec2(0.0f, 0.0f), // Bottom Left
	vec2(0.0f, 1.0f)  // Top Left 
};

Mico::Mico():running(true),previous(0.0),steps(0.0)
{
	windowSystem = new WindowSystem(800, 600);
	graphicsSystem = new GraphicsSystem( windowSystem->Getsize());
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
	WindowHandler::GetInstance()->addObserver(graphicsSystem);
	EntityManager::GetInstance()->Initialize();
	graphicsSystem->InitScene();
	//RawModel* rawModel = Loader::GetInstance()->LoadToVAO(vertices, textcoor, vector<vec3>(), indexes);
	//Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/dragonballzsuper.jpg");
	//vector<Texture*> myTextures = { targetTtexture };
	//TexturedModel* myModel = new TexturedModel(rawModel, myTextures);

	//vector<Entity*> targets;
	//Entity* target = new Entity();
	//target->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	//target->SetScale(vec3(1.0f, 1.0f, 1.0f));
	//targets.push_back(target);
	//entities.insert(std::pair <TexturedModel*, vector<Entity*>>(myModel, targets));

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

		
		graphicsSystem->InitRender();
		graphicsSystem->Update();
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
}


