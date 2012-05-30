#pragma once
#include <cassert>
#include "dataTypes.h"
//http://scottbilas.com/publications/gem-resmgr/

template <typename TAG>
class Handle
{
public:
	//Lifetime
	Handle()
		:m_handle(0)
	{}

	void setIndex(uint16 index);
	
	uint16 getIndex() const 
	{  
		return m_index;  
	}

	uint16 getMagic() const
	{
		return m_magic;
	}

	uint32 getHandle() const
	{
		return m_handle;
	}

	bool isNull() const
	{
		return !m_handle;
	}

	operator uint32() const
	{
		return m_handle;
	}

private:
	union
	{
		struct
		{
			uint16 m_index; //index into resource array
			uint16 m_magic; //magic number to check
		};
		uint32 m_handle;
	};
};

#include "Handle.inl"
