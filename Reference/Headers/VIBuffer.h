/*!
 * \file VIBuffer.h
 * \date 2015/04/04 22:28
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
#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResource
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	
private:
	CVIBuffer& operator = (const CVIBuffer&);

public:
	virtual ~CVIBuffer(void);

public:
	virtual HRESULT		CreateBuffer(void);
	virtual CResource*	CloneResource(void) PURE;

public:
	void GetVtxState(void* pVtxState);
	void SetVtxState(void* pVtxState);
	void SetIdxState(void* pIdxState, const DWORD* pTriCnt);
	virtual void ChangeColor(DWORD dwColor) {}
	void Change_BoxColor(DWORD dwColor);


public:
	virtual void Render(const D3DXMATRIX* pmatWorld);

public:
	virtual DWORD Release(void);



protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	DWORD		m_dwVtxSize;
	DWORD		m_dwVtxCnt;
	DWORD		m_dwVtxFVF;

	LPDIRECT3DINDEXBUFFER9		m_pIB;
	DWORD		m_dwIdxSize;
	DWORD		m_dwTriCnt;
	D3DFORMAT	m_IdxFmt;
};

END

#endif // VIBuffer_h__