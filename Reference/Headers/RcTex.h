/*!
 * \file RcTex.h
 * \date 2015/04/04 23:00
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
#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex
	: public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CRcTex(const CRcTex& rhs);

public:
	virtual ~CRcTex(void);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);
	
	virtual CResource* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);
};

END

#endif // RcTex_h__