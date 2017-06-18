#include "stdafx.h"
#include "WhiteCube.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "StaticMesh.h"
#include "JohnStateSubject.h"
#include "JohnStateObserver.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Value.h"

CWhiteCube::CWhiteCube(LPDIRECT3DDEVICE9 pDevice)
: CCube(pDevice)
, m_fDistance(5.f)
{

}

CWhiteCube::~CWhiteCube(void)
{
	Release();
}

CWhiteCube* CWhiteCube::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData)
{
	CWhiteCube* pGameObject = new CWhiteCube(pDevice);

	if(FAILED(pGameObject->InitCube(pCubeData)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}   

HRESULT CWhiteCube::InitCube(const Engine::CUBEDATA* pCubeData)
{
	m_pInput = Engine::Get_Input();
	m_pResourceMgr = Engine::Get_ResourceMgr();
	
	m_pJohnStateObserver = CJohnStateObserver::Create(CJohnStateSubject::GetInstance());
	CJohnStateSubject::GetInstance()->Subscribe(m_pJohnStateObserver);


	FAILED_CHECK(AddComponent());
	
	if(pCubeData)
	{
		m_dwCubeType = pCubeData->dwCubeType;
		memcpy(m_pState->m_fAngle, pCubeData->fAngle, sizeof(float) * 3);
		m_pState->m_vDir = pCubeData->vDir;
		m_pState->m_vPos = pCubeData->vPos;
		m_pState->m_matWorld = pCubeData->matWorld;
	}

	float fCubeScale = CCube::CUBESCALE;
	m_pState->m_vScale = D3DXVECTOR3(fCubeScale, fCubeScale, fCubeScale);

	m_vOrigin = m_pState->m_vPos;
	m_pState->Update();
	Compute_ViewZ(m_pState->m_vPos);




	m_pStaticMesh->GetMinMax(&m_StdVtx3);

	// MID
	D3DXVECTOR3 vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;

	// LEFT
	vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vLeft = (m_StdVtx3._min + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._max.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vLeft, &vLeft, &m_pState->m_matWorld);

	m_vLeft = vLeft - vCenter;
	D3DXVec3Normalize(&m_vLeft, &m_vLeft);


	// RIGHT
	vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vRight = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._max.x, m_StdVtx3._min.y, m_StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter,  &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vRight, &vRight,  &m_pState->m_matWorld);

	m_vRight = vRight - vCenter;
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	srand(UINT(Engine::Get_TimeMgr()->GetTime()));

	m_iMulti = (rand() % 1 + 1);
	return S_OK;
}

HRESULT CWhiteCube::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_WHITE);
	m_pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATIC, pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::COLBUFFER_SPHERE);
	m_pSphere = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pSphere->ChangeColor(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	m_mapComponent.insert(MAPCOMPONENT::value_type(COLBUFFER_SPHERE, pComponent));

	return S_OK;
} 

void CWhiteCube::Update(void)
{  
	if(!m_IsUpdate)
		return;

	//D3DXVECTOR3 vPos;
	//m_pJohnStateObserver->GetData(CJohnStateSubject::DATAMESSAGE_POS, (void*)&vPos, CJohnStateSubject::DATATYPE_FLOAT, sizeof(D3DXVECTOR3));

	Engine::CGameObject::Update();
	Compute_ViewZ(m_pState->m_vPos);
}

void CWhiteCube::Render(void)
{
	if(!m_IsUpdate)
		return;

	m_pStaticMesh->RenderMesh(&m_pState->m_matWorld);
}

void CWhiteCube::Release(void)
{
	Engine::Safe_Delete(m_pJohnStateObserver);
}