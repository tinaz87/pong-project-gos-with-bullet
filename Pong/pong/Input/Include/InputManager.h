#pragma once
#include "dataTypes.h"

typedef MSG OS_MSG;

class InputManager
{
public:
	InputManager();
	virtual ~InputManager();
	void setFilter(uint16 minMsg, uint16 maxMsg);
	virtual bool handleOsInputEvent(const OS_MSG& msg)= 0;

protected:
	uint16	m_filterMinMsg;
	uint16  m_filterMaxMsg;
};