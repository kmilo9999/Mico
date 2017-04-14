#pragma once
#include "BoundingVolumen.h"
class SelectableBoundingVolumen: public BoundingVolumen
{
public:
	SelectableBoundingVolumen();
	~SelectableBoundingVolumen();
	virtual bool IsSelected() { return selected; };


protected:
	bool selected;
};

