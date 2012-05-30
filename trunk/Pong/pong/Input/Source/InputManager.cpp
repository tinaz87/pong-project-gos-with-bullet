#include "InputManager.h"

InputManager::InputManager()
	:m_filterMinMsg(0)
	,m_filterMaxMsg(uint16MAX)
{

}

InputManager::~InputManager()
{

}

void InputManager::setFilter(uint16 minMsg, uint16 maxMsg)
{
	m_filterMinMsg= minMsg;
	m_filterMaxMsg= maxMsg;
}