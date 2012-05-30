#pragma once
//see http://www.qsoftz.com/mirza/?p=59
#include "CustomMemory.h"

template <typename T, size_t N=16>
class AAllocator
{
public:
	typedef	T			value_type;
	typedef	size_t		size_type;
	typedef	ptrdiff_t	difference_type;
	typedef	T			*pointer;
	typedef const T		*const_pointer;
	typedef	T			&reference;
	typedef const T		&const_reference;
	inline AAllocator() throw(){}

	template <typename T2>
	inline  AAllocator(const AAllocator<T2, N> &) throw(){}
	inline ~AAllocator() throw(){}	

	inline pointer adress(reference r)
	{ return &r; }

	inline const_pointer adress(const_reference r) const
	{ return &r; }

	//std allocator interface --------------------------------
	inline pointer allocate(size_type n)
	{ return (pointer)MV_MALLOC(n*sizeof(value_type), N); }

	inline void deallocate(pointer p, size_type)
	{ allocator::GetSingletonPtr()->free(p); }

	inline void construct(pointer p, const value_type & wert)
	{ new(p) value_type(wert); }

	inline void destroy(pointer p)
	{ /* C4100 */ p; p->~value_type(); }
	//--------------------------------------------------------

	inline size_type max_size() const throw()
	{ return size_type(-1) / sizeof(value_type); }

	template <typename T2>
	struct rebind { typedef AAllocator<T2, N> other; };
};
