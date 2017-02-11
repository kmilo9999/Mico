#pragma once

class Event {

   public:
	   enum EventType
	   {
		   Resized,
		   KeyPressed,
		   KeyReleased,
		   MouseButtonPressed,
		   MouseButtonReleased,
		   MouseMoved,
		   MouseWheel
	   };
	   
	   Event(EventType type) :type(type) {};

	   struct SizeEvent
	   {
		   unsigned int width;
		   unsigned int height;
	   };

	   struct KeyEvent{
		   int code;
	   };

	   struct MouseMovementEvent
	   {
		   float x;
		   float y;
	   };

	   struct MouseClickEvent
	   {
		   int button;
		   float x;
		   float y;
	   };
	   
	   struct MouseWheelEvent
	   {
		   float	z;
	   };


	   EventType type;
	   
	   union
	   {
		   SizeEvent			size;
		   KeyEvent			key;
		   MouseMovementEvent		mouseMove;
		   MouseClickEvent	mouseButton;
		   MouseWheelEvent		mouseWheel;
	   };
	   
};