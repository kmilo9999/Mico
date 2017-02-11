#pragma once
#include <string>

class System {

public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual std::string className() = 0;

};
