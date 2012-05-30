
template <typename TAG>
void Handle<TAG>::setIndex(uint16 index)
{
	assert(isNull());             // don't allow reassignment

	static uint16 s_AutoMagic = 0;
	if ( ++s_AutoMagic == 0)
	{
		s_AutoMagic = 1;    // 0 is used for "null handle"
	}

	m_index = index;
	m_magic = s_AutoMagic;
}

