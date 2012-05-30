#pragma once
//see http://scottbilas.com/publications/gem-singleton/

#include <cassert>
#include "CustomMemory.h"

template <typename T> class Singleton
{
	static T* ms_singleton;

public:
	Singleton()
	{
		assert( !ms_singleton);
		ms_singleton = static_cast <T*> (this);
	}

	~Singleton()
	{ 
		assert(ms_singleton); 
		ms_singleton = 0; 
	}

	static T& GetSingleton()
	{ 
		assert(ms_singleton); 
		return *ms_singleton; 
	}

	static T* GetSingletonPtr()
	{ 
		return ms_singleton; 
	}

	static void Release()
	{
		assert(ms_singleton);
		MV_DELETE(ms_singleton);
		ms_singleton= NULL;
	}

};

template <typename T> T* Singleton <T>::ms_singleton = 0;
