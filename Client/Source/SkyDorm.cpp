#include "stdafx.h"
#include "SkyDorm.h"

#include "Enum.h"
#include "Engine_Function.h"
#include "Export_Resources.h"
#include "JohnStateSubject.h"
#include "JohnStateObserver.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Value.h"

CSkyDorm::CSkyDorm(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pState(NULL)
, m_pStaticMesh(NULL)
, m_pJohnStateObserver(NULL)
, m_pJohnStateSubject(NULL)
{

}

CSkyDorm::~CSkyDorm(void)
{
	Release();
}

CSkyDorm* CSkyDorm::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkyDorm* pSkyDorm = new CSkyDorm(pDevice);
	
	if(FAILED(pSkyDorm->InitSkyDorm()))
		Engine::Safe_Delete(pSkyDorm);

	return pSkyDorm;
}

HRESULT CSkyDorm::InitSkyDorm(void)
{
	FAILED_CHECK(AddComponent());
	
	m_pJohnStateSubject = CJohnStateSubject::GetInstance();
	m_pJohnStateObserver = CJohnStateObserver::Create(m_pJohnStateSubject);
	m_pJohnStateSubject->Subscribe(m_pJohnStateObserver);

	//m_pState->m_vScale = D3DXVECTOR3(0.005f, 0.005f, 0.005f);
	m_pState->m_vScale = D3DXVECTOR3(0.009f, 0.009f, 0.009f);
	m_pState->m_vPos.y -= 200.f;
	m_pState->Update();

	return S_OK;
}

HRESULT CSkyDorm::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"Failed to create Transform.");
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCEID::STATICMESH_SKYDORM);
	m_pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"Failed to create SkyDorm.");
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATIC, pComponent));
	
	return S_OK;
}

void CSkyDorm::Update(void)
{
	m_pState->m_fAngle[Engine::ANGLE_Y] += 0.00001f;

	m_pJohnStateObserver->GetData(CJohnStateSubject::DATAMESSAGE_POS, (void*)&m_pState->m_vPos, CJohnStateSubject::DATATYPE_FLOAT, sizeof(D3DXVECTOR3));

	m_pState->m_vPos.y -= 200.f;

	Engine::CGameObject::Update();	
}

void CSkyDorm::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pStaticMesh->RenderMesh(&m_pState->m_matWorld);
	
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CSkyDorm::Release(void)
{
	m_pJohnStateSubject->Unsubscribe(m_pJohnStateObserver->GetIndex(), m_pJohnStateObserver);
	Engine::Safe_Delete(m_pJohnStateObserver);
}

