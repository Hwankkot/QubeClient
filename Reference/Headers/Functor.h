#pragma once

class CStringCompare
{
private:
	const TCHAR*	m_pString;
public:
	explicit	CStringCompare(const TCHAR*	pKey)
		:m_pString(pKey){;}
	~CStringCompare(){;}
public:
	template<typename T>
	bool operator () (T data)
	{
		return !lstrcmp(data.first, m_pString);
	}
};