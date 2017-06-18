
/*!
 * \file Component.h
 * \date 2016/04/10 15:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
protected:
	explicit CComponent(void);

public:
	virtual ~CComponent(void);

public:
	virtual void Update(void) {}
	virtual DWORD Release(void) PURE;
};

END

#endif // Component_h__