
/*!
 * \file JohnStateSubject.h
 * \date 2016/05/07 3:05
 *
 * \author Architect
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef JohnStateSubject_h__
#define JohnStateSubject_h__

#include "Engine_Macro.h"
#include "Subject.h"

class CJohnStateSubject : public Engine::CSubject
{
	DECLARE_SINGLETON(CJohnStateSubject)

public:
	enum DATATYPE {	DATATYPE_FLOAT };
	enum DATAMESSAGE { DATAMESSAGE_POS, DATAMESSAGE_END };

private:
	CJohnStateSubject(void);
	~CJohnStateSubject(void);

public:
	virtual void AddData(const DWORD& dwMessage, const DWORD& dwDataType, const DWORD& dwDataSize, const DWORD& dwContainerSize = 0);
	void GetData(const DWORD& dwMessage, void* pDataStorage, const DWORD& dwDataSize);
	void NotifyJohnState(const DWORD& dwMessage, void* pDataSource, const DWORD& dwDataSize);

private:
	void Release(void);

private:
	typedef std::map<DWORD, void*> DATAMAP;
	DATAMAP	m_mapDataContainer;
};

#endif // JohnStateSubject_h__