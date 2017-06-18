/*!
 * \file Subject.h
 * \date 2016/04/18 17:37
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
#ifndef Subject_h__
#define Subject_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CObserver;
class ENGINE_DLL CSubject
{
protected:
	CSubject(void);
	
private:
	explicit CSubject(const CSubject&);
	CSubject& operator = (const CSubject&);

protected:
	virtual ~CSubject(void);

public:
	virtual void Subscribe(CObserver* pObserver);
	virtual void Unsubscribe(const DWORD& dwIndex, const CObserver* pObserver);
	
protected:
	virtual void Notify(const DWORD& dwMessage);

public:
	virtual void AddData(const DWORD& dwMessage, const DWORD& dwDataType
		, const DWORD& dwDataSize, const DWORD& dwContainerSize) PURE;

private:
	void Release(void);

protected:
	typedef std::vector<CObserver*>   OBSERVERLIST;
	OBSERVERLIST m_ObserversList;
};

END

#endif // Subject_h__