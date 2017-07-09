#include "SelectableBoundingVolumen.h"
#include <iostream>


SelectableBoundingVolumen::SelectableBoundingVolumen():selected(false)
{
	
}


SelectableBoundingVolumen::~SelectableBoundingVolumen()
{
}

void SelectableBoundingVolumen::Update()
{
	
}

void SelectableBoundingVolumen::Initialize(Entity* owner)
{
	myOwner = owner;
}

void SelectableBoundingVolumen::ShutDown()
{
}
