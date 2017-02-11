#include "Camera.h"
#include <math.h>


Camera::Camera(vec3 Position, float xAngle, float yAngle) :
	speed(3.0f), mouseSpeed(0.005f), foV(45.0f),inputFlag(MovingFlag::stop),lastMousePosition(0.0f, 0.0f),
	canMove(false)
{
	
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
	up = normalize(cross(up, front));
	

}

void Camera::RotateY(float angle)
{
	vec3 Haxis = cross(yAxis, front);
	Haxis = normalize(Haxis);

	rotate(front, angle, yAxis);
	front = normalize(front);
	up = normalize(cross(up, front));

}

void Camera::rotate(vec3& vector, float angle, const vec3& axis)
{
	float angleRadians = (angle / 2) * (180 / PI);
	float halfSin = sinf(angleRadians);
	float halfCos = cosf(angleRadians);
	quat rotation(halfCos, axis.x * halfSin, axis.y * halfSin, axis.z * halfSin);
	quat conjugate = inverse(rotation);
	quat w = rotation * vector * conjugate;

	vector.x = w.x;
	vector.y = w.y;
	vector.z = w.z;

}

mat4& Camera::GetView()
{
	// Camera matrix
	return glm::lookAt(position, front, up);
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
				break;

			float xAngle = (evt.mouseMove.x - lastMousePosition.x)*mouseSpeed;
			float yAngle = (evt.mouseMove.y - lastMousePosition.y)*mouseSpeed;
			RotateX(xAngle);
			RotateY(yAngle);
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

}






