#include "KeyboardInputManager.h"
#include "Glog/logging.h"

void KeyboardInputManager::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		MV_NEW KeyboardInputManager();
}

KeyboardInputManager::KeyboardInputManager()
{
	setFilter(WM_KEYFIRST, WM_KEYLAST);
	m_inputData.resize(uint8MAX);
	reset();	
}

KeyboardInputManager::~KeyboardInputManager()
{
}

bool KeyboardInputManager::handleOsInputEvent(const OS_MSG& msg)
{
	if(msg.message >= m_filterMinMsg && msg.message <= m_filterMaxMsg)
	{
		// grab bits 16-23 from LPARAM
		uint8 oemScan= int(msg.lParam & (0xff << 16))>>16;
		uint8 asciiId= MapVirtualKey(oemScan, MAPVK_VSC_TO_VK_EX);

		if(msg.message == MSG_DOWN)
		{
			DLOG_IF(INFO, m_inputData[asciiId] != KEY_DOWN) << "key " << (char)asciiId << " down";
			m_inputData[asciiId]= KEY_DOWN;
		}
		else if(msg.message == MSG_RELEASED)
		{
			m_inputData[asciiId]= KEY_UP;			
			DLOG(INFO) << "key " << (char)asciiId << " released";
		}
		return true;
	}
	return false;
}

uint8 KeyboardInputManager::getKeyState(uint8 key) const
{
	return m_inputData[key];
}

void KeyboardInputManager::reset()
{
	for(uint32 i= 0; i < m_inputData.size(); ++i)
	{
		m_inputData[i]= 0;
	}	
}