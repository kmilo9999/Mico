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
		   MouseWheel,
		   UI
	   };
	   
	   enum UIEventType
	   {
		   ShowNormals,
		   ChangeModel,
		   MaterialAmbient,
		   MaterialDiffuse,
		   MaterialSpecular,
		   GoochWarmColor,
		   GoochColdColor,
		   GoochAlpha,
		   GoochBeta,
		   CelBrightLevel,
		   ShowModel
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

	   union UIEventValue
	   {
		   bool boolValue;
		   int modelIndex;
	   };


	   struct UIEvtType
	   {
		   UIEventType type;
		   UIEventValue value;
		   float ambientColorR;
		   float ambientColorG;
		   float ambientColorB;
		   float diffuseColorR;
		   float diffuseColorG;
		   float diffuseColorB;
		   float specularColorR;
		   float specularColorG;
		   float specularColorB;
		   float goochWarmColorR;
		   float goochWarmColorG;
		   float goochWarmColorB;
		   float goochColdColorR;
		   float goochColdColorG;
		   float goochColdColorB;
		   float goochAlpha;
		   float goochBeta;
		   float brightLevel;
	   };

	   EventType type;
	   
	   union
	   {
		   SizeEvent			size;
		   KeyEvent			key;
		   MouseMovementEvent		mouseMove;
		   MouseClickEvent	mouseButton;
		   MouseWheelEvent		mouseWheel;
		   UIEvtType ui;
	   };
	   
	
	 

};
