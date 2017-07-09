#pragma once
#include "BoundingVolumen.h"
class SelectableBoundingVolumen: public BoundingVolumen
{
public:
	SelectableBoundingVolumen();
	~SelectableBoundingVolumen();
	virtual bool IsSelected() { return selected; };
	virtual void Update();

	virtual void Initialize(Entity* owner);
	virtual void ShutDown();

protected:
	bool selected;
};

