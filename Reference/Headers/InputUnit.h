/*!
 * \file InputUnit.h
 * \date 2016/04/19 10:37
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
#ifndef InputUnit_h__
#define InputUnit_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CInputUnit : public CComponent
{
private:
	CInputUnit(void);

public:
	~CInputUnit(void);

public:
	static CInputUnit* Create(void);

	virtual void Update(void);

	virtual DWORD Release(void);


};

END

#endif // InputUnit_h__