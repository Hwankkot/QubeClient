#include "VIBuffer.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: CResource(pDevice)
,m_pVB(NULL)
,m_dwVtxSize(0)
,m_dwVtxCnt(0)
,m_dwVtxFVF(0)
,m_pIB(NULL)
,m_dwIdxSize(0)
,m_dwTriCnt(0)
,m_IdxFmt(D3DFMT_UNKNOWN)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
: CResource(rhs.m_pDevice)
, m_pVB(rhs.m_pVB)
, m_dwVtxSize(rhs.m_dwVtxSize)
, m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwVtxFVF(rhs.m_dwVtxFVF)
, m_pIB(rhs.m_pIB)
, m_dwIdxSize(rhs.m_dwIdxSize)
, m_dwTriCnt(rhs.m_dwTriCnt)
, m_IdxFmt(rhs.m_IdxFmt)
{

}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void Engine::CVIBuffer::GetVtxState(void* pVtxState)
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pVtxState, pOriVtx, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetVtxState(void* pVtxState)
{
	void* pOriVtx = NULL;
	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pOriVtx, pVtxState, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetIdxState(void* pIdxState, const DWORD* pTriCnt)
{
	void* pOriIndex = NULL;

	m_dwTriCnt = *pTriCnt;

	m_pIB->Lock(0, 0, &pOriIndex , 0);
	memcpy(pOriIndex, pIdxState, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();
}

void Engine::CVIBuffer::Change_BoxColor(DWORD dwColor)
{
	VTXCOL*	pVertex= NULL;
	
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	for(int i = 0; i < 8; ++i)
		pVertex[i].dwColor = dwColor;
	
	m_pVB->Unlock();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	HRESULT hr = NULL;
	hr = m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"VertexBuffer Create Failed");

	hr = m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"IndexBuffer Create Failed");
	
	return S_OK;
}

void Engine::CVIBuffer::Render(const D3DXMATRIX* pmatWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pmatWorld);

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

DWORD Engine::CVIBuffer::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Engine::Safe_Release(m_pVB);
		Engine::Safe_Release(m_pIB);

		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}