#include "Camera.h"
#include <math.h>
#include <iostream>

Camera::Camera(vec3 Position,vec3 front, vec3 up) :position(Position),
	speed(0.1f), mouseSpeed(0.2f), foV(45.0f),inputFlag(MovingFlag::stop),lastMousePosition(0.0f, 0.0f),
	canMove(false), front(front),up(up)
{
	front = normalize(front);
	up = normalize(up);
}

Camera::~Camera()
{

}

void Camera::Move(vec3 dir, float dt)
{
	position += front * dt * speed;
}

void Camera::RotateX(float angle)
{
	vec3 Haxis = cross(yAxis, front);
	Haxis = normalize(Haxis);

	rotate(front, angle, Haxis);
	front = normalize(front);
	up = normalize(cross(front, Haxis));
	

}

void Camera::RotateY(float angle)
{
	vec3 Haxis = cross(yAxis, front);
	Haxis = normalize(Haxis);

	rotate(front, angle, yAxis);
	front = normalize(front);
	up = normalize(cross(front, Haxis));

}

void Camera::rotate(vec3& vector, float angle, const vec3& axis)
{
	quat quat_view(0, vector.x, vector.y, vector.z);

	float angleRadians = (angle) * (PI / 180);
	float halfSin = sinf(angleRadians /2);
	float halfCos = cosf(angleRadians / 2);
	quat rotation(halfCos, axis.x * halfSin, axis.y * halfSin, axis.z * halfSin);
	quat conjugate = inverse(rotation);
	quat q =  rotation * quat_view;
	q *= conjugate;

	vector.x = q.x;
	vector.y = q.y;
	vector.z = q.z;

}

mat4& Camera::GetView()
{
	// Camera matrix
	return glm::lookAt(position, position + front, up);
}

vec3 Camera::GetFront()
{
	return position;
}

void Camera::onNotify(Event& evt)
{
	switch (evt.type)
	{
	case Event::KeyPressed:
		switch (evt.key.code)
		{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			inputFlag = MovingFlag::forward;
			break;

		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			inputFlag = MovingFlag::backward;
			break;

		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			inputFlag = MovingFlag::left;
			break;

		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			inputFlag = MovingFlag::right;
			break;
		};
		break;
	case Event::KeyReleased:
		switch (evt.key.code)
		{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			inputFlag &= !MovingFlag::forward;
			break;

		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			inputFlag &= !MovingFlag::backward;
			break;

		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			inputFlag &= !MovingFlag::left;
			break;

		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			inputFlag &= !MovingFlag::right;
			break;
		};
		break;
	case Event::MouseMoved:
		{
			if (!canMove)
			{
				lastMousePosition.x = evt.mouseMove.x;
				lastMousePosition.y = evt.mouseMove.y;
				break;
			}	

			float xAngle = (evt.mouseMove.x - lastMousePosition.x)*mouseSpeed;
			float yAngle = (evt.mouseMove.y - lastMousePosition.y)*mouseSpeed;
			RotateX(yAngle); // invert the angle because to feel it more natural
			RotateY(-xAngle);
			lastMousePosition.x = evt.mouseMove.x;
			lastMousePosition.y = evt.mouseMove.y;
		}
		break;
	case Event::MouseButtonPressed:
		switch (evt.mouseButton.button)
		{
			case GLFW_MOUSE_BUTTON_RIGHT:
				canMove = true;
				break;
		};
		break;
	case Event::MouseButtonReleased:
		switch (evt.mouseButton.button)
		{
		case GLFW_MOUSE_BUTTON_RIGHT:
			canMove = false;
			break;
		};
		break;
	default:
		break;
	}
}

void Camera::Update()
{
	if (inputFlag == MovingFlag::forward)
	{
		position += front * speed;
	}
	if (inputFlag == MovingFlag::backward)
	{
		position -= front * speed;
	}
	if (inputFlag == MovingFlag::right)
	{
		position += glm::normalize(glm::cross(front, up)) * speed;
	}
	if (inputFlag == MovingFlag::left)
	{
		position -= glm::normalize(glm::cross(front, up)) * speed;
	}
}






