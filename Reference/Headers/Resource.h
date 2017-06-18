/*!
 * \file Resource.h
 * \date 2016/04/03 16:51
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

#ifndef Resource_h__
#define Resource_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResource
	: public CComponent
{
protected:
	explicit CResource(LPDIRECT3DDEVICE9 pDevice);
	explicit CResource(const CResource& rhs);

public:
	virtual ~CResource(void);

public:
	/// Interface of creating a clone.
	virtual CResource* CloneResource(void)PURE;
	virtual DWORD Release(void) PURE;

public:
	/// Add Reference count.
	void AddRefCnt(void);

protected:
	DWORD		m_dwRefCnt;

protected:
	LPDIRECT3DDEVICE9		m_pDevice;
};

END

#endif // Resource_h__