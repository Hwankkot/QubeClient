#include "stdafx.h"
#include "PickingParticle.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Transform.h"
#include "Texture.h"
#include "Value.h"
#include "VIBuffer.h"
#include "Struct.h"


CPickingParticle::CPickingParticle(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pTimeMgr(NULL)
, m_fTexutreIndex(0.f)
, m_IsEnd(false)
{
   
}

CPickingParticle::~CPickingParticle(void)
{

}

CPickingParticle* CPickingParticle::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vPos)
{
	CPickingParticle* pParticle = new CPickingParticle(pDevice);

	if(FAILED(pParticle->InitPickingPartcle(vPos)))
		Engine::Safe_Delete(pParticle);

	return pParticle;
}

HRESULT CPickingParticle::InitPickingPartcle(const D3DXVECTOR3& vPos)
{
	m_pTimeMgr = Engine::Get_TimeMgr();

	FAILED_CHECK(AddComponent());

	D3DXMatrixIdentity(&m_matWorld);
	memcpy(&m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW,&matView);

	D3DXMATRIX matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = matView;
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_matWorld = matBill * m_matWorld;

		
	return S_OK;
}

HRESULT CPickingParticle::AddComponent(void)
{
	Engine::CComponent*	pComponent = NULL;

	pComponent = Engine::CResourceMgr::GetInstance()->CloneResource(RESOURCEID::RECTTEXTURE_PICKINGPARTICLE);
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(RECTTEXTURE_PARTICLE, pComponent));

	pComponent = Engine::CResourceMgr::GetInstance()->CloneResource(RESOURCEID::TEXBUFFER_RECTANGLE);
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TEXBUFFER_PARTICLE, pComponent));
	
	return S_OK;
}

void CPickingParticle::Update(void)
{
	m_fTexutreIndex += (19.f *  m_pTimeMgr->GetTime() * 3.f);

	if(m_fTexutreIndex > 19.f)
		m_IsEnd = true;

	//SetTransform();
}

void CPickingParticle::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pTexture->Render(0, (DWORD)m_fTexutreIndex);
	m_pBuffer->Render(&m_matWorld);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPickingParticle::SetTransform(void)
{

	// 빌보드는 나를 바라보게 하는 것이 아니라 회전을 상쇄 시키는 것...

	// 위치를 받아오는 걸 옵저버로 교체 가능함.
	D3DXMATRIX matView, matRotY, matRotX, matWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matRotX);

	D3DXVECTOR3 vRight, vUp, vLook, vJohnPos, vParticlePos, vDir;

	memcpy(&vJohnPos,	&matView.m[3][0], sizeof(D3DXVECTOR3));

	memcpy(&vRight,		  &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&vUp,		  &m_matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vLook,		  &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));
	memcpy(&vParticlePos, &m_matWorld.m[3][0], sizeof(D3DXVECTOR3));


	vDir = vJohnPos - vParticlePos;
	D3DXVec3Normalize(&vDir, &vDir);
		
	D3DXVECTOR2 v2Dir(vDir.x, vDir.z);
	D3DXVECTOR2 v2ParticleDir(vLook.x, vLook.z);

	float fCos1 = D3DXVec2Dot(&v2ParticleDir, &v2Dir);
	float fCos2 = D3DXVec2Dot(&v2Dir, &v2ParticleDir);
	float fCos;

	

	if(fCos1 > fCos2)
		fCos = fCos2;
	else
		fCos = fCos1; 

	float fSin = sinf(acosf(fCos));

	matRotY.m[0][0] = fCos;
	matRotY.m[0][2] = fSin;

	matRotY.m[2][0] = -fSin;
	matRotY.m[2][2] = fCos;


	v2Dir         = D3DXVECTOR2(vDir.y, vDir.z);
	v2ParticleDir = D3DXVECTOR2(vLook.y, vLook.z);

	fCos1 = D3DXVec2Dot(&v2ParticleDir, &v2Dir);
	fCos2 = D3DXVec2Dot(&v2Dir, &v2ParticleDir);

	if(fCos1 > fCos2)
		fCos = fCos2;
	else
		fCos = fCos1;

	 fSin = sinf(acosf(fCos));

	matRotX.m[1][1] = fCos;
	matRotY.m[1][2] = -fSin;

	matRotX.m[2][1] = fSin;
	matRotY.m[2][2] = fCos;

	matWorld = matRotX * matRotY;

	memcpy(&vRight,				&matWorld.m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&vUp,				&matWorld.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&vLook,				&matWorld.m[2][0], sizeof(D3DXVECTOR3));
	memcpy(&matWorld.m[3][0],	&m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	/*
	vRight	*= fRight;
	vUp		*= fUp;
	vLook	*= fLook;*/

	m_matWorld = matWorld;
	
	int i = 0;
}
