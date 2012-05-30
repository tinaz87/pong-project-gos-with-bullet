#pragma once
#include "hpha.h"

//Implementation operator for MV_NEW
inline void* operator new(const size_t i_size, const char* const i_fileName, const unsigned int i_line, const char* const i_funcName)
{
	return allocator::GetSingletonPtr()->alloc(i_size);
}

//delete on exception (no constructor called) (see http://msdn.microsoft.com/en-us/library/cxdxz3x6%28v=vs.80%29.aspx)
inline void operator delete(void* ptr, const char* const i_fileName, const unsigned int i_line, const char* const i_funcName)
{
	allocator::GetSingletonPtr()->free(ptr);
}

//Implementation operator for MV_NEW_ALIGNED
inline void* operator new(const size_t i_size, size_t alignment, const char* const i_fileName, const unsigned int i_line, const char* const i_funcName)
{
	return allocator::GetSingletonPtr()->alloc(i_size, alignment);
}

//delete on exception (no constructor called) (see http://msdn.microsoft.com/en-us/library/cxdxz3x6%28v=vs.80%29.aspx)
inline void operator delete(void* ptr, size_t alignment, const char* const i_fileName, const unsigned int i_line, const char* const i_funcName)
{
	allocator::GetSingletonPtr()->free(ptr);
}

inline void* MvAlloc(size_t size) 
{
	return allocator::GetSingletonPtr()->alloc(size);
}

inline void MvFree(void* ptr)
{
	allocator::GetSingletonPtr()->free(ptr);
}

//Aligned malloc
inline void* MvMalloc(size_t size, int alignment) 
{
	return allocator::GetSingletonPtr()->alloc(size, alignment);
}

template<class T> 
inline void MvDelete(T* ptr) 
{
	if (ptr) 
	{
		ptr->~T();
		allocator::GetSingletonPtr()->free(ptr);
	}
}

#define MV_NEW						::new(__FILE__, __LINE__, __FUNCTION__)
#define MV_NEW_ALIGNED(alignment)	::new(alignment, __FILE__, __LINE__, __FUNCTION__)
#define MV_MALLOC(size, alignment)	::MvMalloc(size, alignment)
#define MV_DELETE(ptr)				::MvDelete(ptr)
