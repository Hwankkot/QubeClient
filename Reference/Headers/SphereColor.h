/*!
 * \file SphereColor.h
 * \date 2016/04/27 22:19
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
#ifndef SphereColor_h__
#define SphereColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereColor : public CVIBuffer
{
private:
	explicit CSphereColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CSphereColor(const CSphereColor& rhs);

public:
	virtual ~CSphereColor(void);

public:
	static CSphereColor* Create(LPDIRECT3DDEVICE9 pDevice, const float& fRadius);

public:
	virtual HRESULT		CreateBuffer(const float& fRadius);
	virtual CResource*	CloneResource(void);

public:
	virtual void Render(const D3DXMATRIX* pmatWorld);
	virtual void ChangeColor(DWORD dwColor);

private:
	bool m_IsOrigin;
	DWORD m_dwColor;
	LPD3DXMESH	m_pSphere;
};

END

#endif // SphereColor_h__
