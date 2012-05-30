#pragma once
#include "Allocator.h"
#include <vector>
#include <map>
#include <hash_map>

template<class T>
class MvVector: public std::vector<T, AAllocator<T>>
{

};

template<class T, class V>
class MvMap: public std::map<T, V, std::less<T>, AAllocator< std::pair<const T, V>>>
{

};

template<class T, class V>
class MvHashMap: public stdext::hash_map<T, V, stdext::hash_compare<T, stdext::less<T>>, AAllocator<stdext::pair<const T, V>>>
{

};
