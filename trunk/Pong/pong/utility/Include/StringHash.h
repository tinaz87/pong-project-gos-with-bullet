#pragma once
//see http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php

#include <map>
#include "dataTypes.h"
#include "Containers.h"

#define REPEAT_1(define,x)  define(x)
#define REPEAT_2(define,x)  define(x)##REPEAT_1(define,AddOne<x>::Value)
#define REPEAT_3(define,x)  define(x)##REPEAT_2(define,AddOne<x>::Value)
#define REPEAT_4(define,x)  define(x)##REPEAT_3(define,AddOne<x>::Value)
#define REPEAT_5(define,x)  define(x)##REPEAT_4(define,AddOne<x>::Value)
#define REPEAT_6(define,x)  define(x)##REPEAT_5(define,AddOne<x>::Value)
#define REPEAT_7(define,x)  define(x)##REPEAT_6(define,AddOne<x>::Value)
#define REPEAT_8(define,x)  define(x)##REPEAT_7(define,AddOne<x>::Value)
#define REPEAT(x,define) REPEAT_##x(define,0)

#define PREFIX(n)	((
#define POSTFIX(n)	^ str[n]) * 16777619u)

#ifdef _DEBUG
#define STRING_HASH_CONSTRUCTOR(n)									\
__forceinline StringHash::StringHash(const char (&str)[n])			\
	: m_hash(REPEAT(n, PREFIX) 2166136261u REPEAT(n, POSTFIX))		\
{																	\
	chechConsistency(std::string(str), m_hash);						\
}
#else
#define STRING_HASH_CONSTRUCTOR(n)									\
	__forceinline StringHash::StringHash(const char (&str)[n])		\
	: m_hash(REPEAT(n, PREFIX) 2166136261u REPEAT(n, POSTFIX))		\
{																	\
}
#endif

class StringHash
{
public:
	STRING_HASH_CONSTRUCTOR(1)
	STRING_HASH_CONSTRUCTOR(2)
	STRING_HASH_CONSTRUCTOR(3)
	STRING_HASH_CONSTRUCTOR(4)
	STRING_HASH_CONSTRUCTOR(5)
	STRING_HASH_CONSTRUCTOR(6)
	STRING_HASH_CONSTRUCTOR(7)
	STRING_HASH_CONSTRUCTOR(8)

	//whit this constant string are considered first, and non-constant ones second
	struct ConstCharWrapper
	{
		inline ConstCharWrapper(const char* str) : m_str(str) {}
		const char* m_str;
	};

	StringHash()
		: m_hash(0)
	{
	}

	StringHash(ConstCharWrapper str)
		: m_hash(CalculateFNV(str.m_str))
	{
#ifdef _DEBUG
		chechConsistency(std::string(str.m_str), m_hash);
#endif
	}

	StringHash(const StringHash& stringHash)
	{
		m_hash= stringHash.m_hash;
	}

	uint32 GetHash() const
	{
		return m_hash;
	}

	bool operator<(const StringHash& other) const
	{
		return m_hash < other.m_hash;
	}

	bool operator==(const StringHash& other) const
	{
		return m_hash == other.m_hash;
	}

	bool operator!=(const StringHash& other) const
	{
		return m_hash != other.m_hash;
	}


#ifdef _DEBUG
	static void clearDebug()
	{
		if(m_checkMap)
			delete(m_checkMap);
	}
#endif

private:
	uint32 m_hash;
	
	static uint32 CalculateFNV(const char* str)
	{
		const size_t length = strlen(str) + 1;
		uint32 hash = 2166136261u;
		for (size_t i=0; i<length; ++i)
		{
			hash ^= *str++;
			hash *= 16777619u;
		}
		return hash;
	}

	template <uint8 N>
	struct AddOne
	{
		static const uint8 Value = N + 1;
	};

#ifdef _DEBUG
	static MvMap<uint32, std::string>* m_checkMap;

	static void chechConsistency(const std::string &str, uint32 hash)
	{
		if(m_checkMap==NULL)
		{
			m_checkMap= new MvMap<uint32, std::string>();
		}

		std::pair<MvMap<uint32, std::string>::iterator, bool> insert= m_checkMap->insert(std::make_pair(hash, str));
		if (!(insert.second))
		{
			if(strcmp(str.c_str(), (insert.first->second).c_str()) != 0)
				printf("error, two string whit same hash");
		}
	}
#endif

};

