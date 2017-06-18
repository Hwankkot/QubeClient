#include "SphereColor.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"


Engine::CSphereColor::CSphereColor(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
, m_IsOrigin(false)
{

}

Engine::CSphereColor::CSphereColor(const CSphereColor& rhs)
: CVIBuffer(rhs)
{
	/// 복사생성자로 복사 했을 때는 Clone Sphere를 받음
	// 주석입니다. 
	Release();
}

Engine::CSphereColor::~CSphereColor(void)
{

}

Engine::CSphereColor* Engine::CSphereColor::Create(LPDIRECT3DDEVICE9 pDevice, const float& fRadius)
{
	CSphereColor* pBuffer = new CSphereColor(pDevice);

	if(FAILED(pBuffer->CreateBuffer(fRadius)))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}

HRESULT Engine::CSphereColor::CreateBuffer(const float& fRadius)
{
	D3DXCreateSphere(m_pDevice, fRadius, 20, 20, &m_pSphere, NULL);
	
	m_IsOrigin = true;

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = m_pSphere->GetNumVertices();
	m_dwVtxFVF = VTXFVF_COL;

	m_dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 0.f);

	return S_OK;
}

void Engine::CSphereColor::Render(const D3DXMATRIX* pmatWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pmatWorld);
	m_pSphere->DrawSubset(0);
}

Engine::CResource* Engine::CSphereColor::CloneResource(void)
{
	/// 데이터를 복사할 클론 객체를 만든다.
	CSphereColor* pCloneColor = new CSphereColor(*this);

	/// 클론 메시를 만든다.
	LPD3DXMESH pCloneSphere;
	m_pSphere->CloneMeshFVF(0, m_dwVtxFVF, m_pDevice, &pCloneSphere);

	/// 클론 메시의 주소를 클론 객체에 넣는다.
	pCloneColor->m_pSphere = pCloneSphere;
	pCloneColor->m_IsOrigin = false;

	pCloneSphere->GetVertexBuffer(&pCloneColor->m_pVB);

	pCloneColor->AddRefCnt();

	// 여기도 수정
	return  pCloneColor;
}

void Engine::CSphereColor::ChangeColor(DWORD dwColor)
{
	if(m_IsOrigin)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVB = m_pVB;

	DWORD dwVtxNum = m_dwVtxCnt;
	VTXCOL* pVertex = NULL;
	
	pVB->Lock(0, 0, (void**)&pVertex, NULL);

	for(DWORD i = 0; i < dwVtxNum; ++i)
		pVertex[i].dwColor = dwColor;

	pVB->Unlock();
}
