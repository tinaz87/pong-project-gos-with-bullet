#pragma once
#include "Component.h"

class PositionControllerComponent: public Component
{
public:
	static const StringHash POSITION_CONTROLLER_ID;
	PositionControllerComponent();
	virtual ~PositionControllerComponent();
	virtual void update(real frametime, real timestep);

private:
};