#pragma once
#include "Singleton.h"
#include "InputManager.h"
#include "dataTypes.h"
#include "Containers.h"

typedef MSG OS_MSG;

enum KeyMsg
{
	MSG_DOWN= WM_KEYDOWN,
	MSG_RELEASED= WM_KEYUP,
	MSG_ID= WM_CHAR
};

enum KeyState
{
	KEY_UP= 0,
	KEY_DOWN= uint8MAX
};

class KeyboardInputManager: public InputManager, public Singleton<KeyboardInputManager>
{
public:
	static void  CreateInstance();
	virtual		 ~KeyboardInputManager();
	virtual bool handleOsInputEvent(const OS_MSG& msg);
	uint8		 getKeyState(uint8 key) const;	
	void		 reset();

private:
	KeyboardInputManager();
	MvVector<uint8>	m_inputData;
};