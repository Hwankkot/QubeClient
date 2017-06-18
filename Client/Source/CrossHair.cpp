#include "stdafx.h"
#include "CrossHair.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "JohnStateSubject.h"
#include "Layer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Value.h"

CCrossHair::CCrossHair(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_fSizeX(32.f)
, m_fSizeY(32.f)
, m_vCrossHair(float(WINCX >> 1), float(WINCY >> 1))
, m_pCrossHairTex(NULL)
, m_pCrossHairBuffer(NULL)
{

}

CCrossHair::~CCrossHair(void)
{

}

CCrossHair* CCrossHair::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCrossHair* pJohn = new CCrossHair(pDevice);
	if(FAILED(pJohn->InitCrossHair()))
		Safe_Delete(pJohn);

	return pJohn;
}

HRESULT CCrossHair::InitCrossHair(void)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();

	D3DXMatrixIdentity(&m_matOrthoView);
	D3DXMatrixIdentity(&m_matOrthoProj);

	D3DXMatrixOrthoLH(&m_matOrthoProj, WINCX, WINCY, 0.f, 1.f);

	m_matOrthoView._11 = m_fSizeX;
	m_matOrthoView._22 = m_fSizeY;
	m_matOrthoView._33 = 1.f;

	m_matOrthoView._41 =  m_vCrossHair.x - (WINCX >> 1);
	m_matOrthoView._42 = -m_vCrossHair.y + (WINCY >> 1);

	RECT	rcUI = { long( m_vCrossHair.x - m_fSizeX * 0.5f)
				   , long( m_vCrossHair.y - m_fSizeY * 0.5f)
				   , long( m_vCrossHair.x + m_fSizeX * 0.5f)
				   , long( m_vCrossHair.y + m_fSizeY * 0.5f)};


	FAILED_CHECK(AddComponent());

	ShowCursor(false);

	return S_OK;
}

HRESULT CCrossHair::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::TEXBUFFER_RECTANGLE);
	m_pCrossHairBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TEXBUFFER_CROSSHAIR, pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::RECTTEXTURE_CROSSHAIR);
	m_pCrossHairTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(RECTTEXTURE_CROSSHAIR, pComponent));

	return S_OK;
}


void CCrossHair::Update(void)
{

}

void CCrossHair::Render(void)
{	
	D3DMATRIX matView, matProj;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matOrthoView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matOrthoProj);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pCrossHairTex->Render(0, 0);
	m_pCrossHairBuffer->Render(&matWorld);

	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}