/*!
 * \file JohnStateObserver.h
 * \date 2016/05/07 3:03
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
#ifndef JohnStateObserver_h__
#define JohnStateObserver_h__

#include "Observer.h"
#include "JohnStateSubject.h"

class CJohnStateObserver : public Engine::CObserver
{
private:
	CJohnStateObserver(void);

public:
	~CJohnStateObserver(void);

public:
	static CJohnStateObserver* Create(CJohnStateSubject* pJohnStateSubject);

private:
	HRESULT InitJohnStateObserver(CJohnStateSubject* pJohnStateSubject);

public:
	virtual void Update(const DWORD& dwMessage);
	void GetData(const DWORD& dwMessage, void* const pDataStorage, const DWORD& dwDataType, const DWORD& dwDataSize);

public:
	bool GetCheckDataChanged(const DWORD& dwMessage);

private:
	void Release(void);

private:
	CJohnStateSubject* m_pJohnStateSubject;

private:
	bool m_IsDataChanged[CJohnStateSubject::DATAMESSAGE_END];
	void* m_pData[CJohnStateSubject::DATAMESSAGE_END];
};

#endif // JohnStateObserver_h__