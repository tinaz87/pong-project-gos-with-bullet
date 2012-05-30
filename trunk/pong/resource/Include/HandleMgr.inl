
template <typename DATA, typename HANDLE>
DATA* HandleMgr<DATA, HANDLE>::acquire(HANDLE& handle )
{
	// if free list is empty, add a new one otherwise use first one found
	uint16 index;
	if (m_FreeSlots.empty() )
	{
		index= m_MagicNumbers.size();
		handle.setIndex(index);
		m_UserData.push_back(DATA());
		m_MagicNumbers.push_back(handle.getMagic());
	}
	else
	{
		index = m_FreeSlots.back();
		handle.setIndex( index );
		m_FreeSlots.pop_back();
		m_MagicNumbers[index] = handle.getMagic();
	}
	return &m_UserData[index];
}

template <typename DATA, typename HANDLE>
void HandleMgr <DATA, HANDLE>::release(HANDLE handle)
{
	// which one?
	uint16 index= handle.getIndex();

	// make sure it's valid
	assert( index < m_UserData.size() );
	assert( m_MagicNumbers[index] == handle.getMagic());

	// ok remove it - tag as unused and add to free list
	m_MagicNumbers[index] = 0;
	m_FreeSlots.push_back( index );
}

template <typename DATA, typename HANDLE>
inline DATA* HandleMgr <DATA, HANDLE>::dereference(HANDLE handle)
{
	if(handle.isNull())
		return 0;

	// check handle validity - this check can be removed for speed
	// if you can assume all handle references are always valid.
	uint16 index = handle.getIndex();
	if ( (index >= m_UserData.size() ) || 
		 ( m_MagicNumbers[index] != handle.getMagic() ) )
	{
		// no good! invalid handle == client programming error
		assert(0);
		return 0;
	}

	return &m_UserData[index];
}

template <typename DATA, typename HANDLE>
inline const DATA* HandleMgr <DATA, HANDLE>::dereference(HANDLE handle) const
{
	// this lazy cast is ok - non-const version does not modify anything
	typedef HandleMgr<DATA, HANDLE> ThisType;
	return ( const_cast <ThisType*> ( this )->dereference( handle ) );
}