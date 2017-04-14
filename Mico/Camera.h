#pragma once
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include "Observer.h"

#define PI 3.14159265f


using namespace glm;
class Camera:public Observer
{
	enum MovingFlag
	{
		stop = 0,
		forward,
		backward,
		right,
		left
	};

public:
	Camera(vec3 Postion = vec3(0.0f, 0.0f, 0.0f),vec3 front =  vec3(0.0f, 0.0f, -1.0f),vec3 Up = vec3(0.0,1.0,0.0));
	~Camera();
	
	void Move(vec3 dir, float dt);
	void RotateX(float angle);
	void RotateY(float angle);
	void Update();

	virtual void onNotify(Event& event);

	vec3 position;
	vec3 front;
	vec3 up;
	
	mat4& GetView();
	vec3 GetFront();
	
	float speed;
	float mouseSpeed;
	float horizontalAngle;
	float verticalAngle;
	float foV;

	int windowWidth;
	int windowHeight;

private:
	const vec3 yAxis =  vec3(0.0f,1.0f,0.0f);
	unsigned int  inputFlag;
	void rotate(vec3& vector, float angle,const vec3& axis);
	vec2 lastMousePosition;
	bool canMove;
	
	
};

