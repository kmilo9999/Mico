#pragma once
#include <map>
#include "System.h"

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
};

