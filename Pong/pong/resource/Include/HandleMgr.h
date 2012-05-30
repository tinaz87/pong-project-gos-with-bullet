#pragma once
#include <cassert>
#include "Handle.h"
#include "Containers.h"

template<typename DATA, typename HANDLE>
class HandleMgr
{
private:
	// private types
	typedef MvVector<DATA>   UserVec;
	typedef MvVector<uint16> MagicVec;
	typedef MvVector<uint16> FreeVec;

	// private data
	UserVec  m_UserData;     // data we're going to get to
	MagicVec m_MagicNumbers; // corresponding magic numbers
	FreeVec  m_FreeSlots;    // keeps track of free slots in the db

public:
	//Lifetime
	HandleMgr() {  }
	~HandleMgr() {  }

	// Handle methods
	// acquisition
	DATA* acquire(HANDLE& handle);
	void  release(HANDLE  handle);

	// dereferencing
	DATA*       dereference(HANDLE handle);
	const DATA* dereference(HANDLE handle) const;

	// other query
	uint16 getUsedHandleCount( void ) const
	{  return (m_MagicNumbers.size() - m_FreeSlots.size() );}
	bool hasUsedHandles( void ) const
	{  return ( !!GetUsedHandleCount() );  }
};

#include "HandleMgr.inl"