/*!
 * \file Observer.h
 * \date 2016/04/18 17:30
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
#ifndef Observer_h__
#define Observer_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CObserver
{
protected:
	explicit CObserver(void);

private:
	explicit CObserver(const CObserver&);
	CObserver& operator = (const CObserver&);

protected:
	virtual ~CObserver(void);

public:
	virtual void Update(const DWORD& dwMessage) PURE;
	void SetIndex(const DWORD& dwIndex);
	DWORD GetIndex(void);

protected:
	DWORD m_dwIndex;
};

END


#endif // Observer_h__