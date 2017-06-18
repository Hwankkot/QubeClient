#include "stdafx.h"
#include "GreenCube.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Value.h"

CGreenCube::CGreenCube(LPDIRECT3DDEVICE9 pDevice)
: CCube(pDevice)
{

}

CGreenCube::~CGreenCube(void)
{
	Release();
}

CGreenCube* CGreenCube::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData)
{
	CGreenCube* pGameObject = new CGreenCube(pDevice);

	if(FAILED(pGameObject->InitCube(pCubeData)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}   

HRESULT CGreenCube::InitCube(const Engine::CUBEDATA* pCubeData)
{
	m_pInput = Engine::Get_Input();
	//m_pRenderer = Engine::Get_Management()->GetRenderer();
	m_pResourceMgr = Engine::Get_ResourceMgr();

	FAILED_CHECK(AddComponent());

	if(pCubeData)
	{
		m_dwCubeType = pCubeData->dwCubeType;
		memcpy(m_pState->m_fAngle, &pCubeData->fAngle, sizeof(float) * 3);
		m_pState->m_vScale = pCubeData->vScale;
		m_pState->m_vDir = pCubeData->vDir;
		m_pState->m_vPos = pCubeData->vPos;
		m_pState->m_matWorld = pCubeData->matWorld;

	}
	else
	{
		float fCubeScale = CCube::CUBESCALE;
		m_pState->m_vScale = D3DXVECTOR3(fCubeScale, fCubeScale, fCubeScale);
	}

	m_pState->Update();
	Compute_ViewZ(m_pState->m_vPos);

	return S_OK;
}

HRESULT CGreenCube::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_GREEN);
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

void CGreenCube::Update(void)
{  
	Engine::CGameObject::Update();
	Compute_ViewZ(m_pState->m_vPos);
}

void CGreenCube::Render(void)
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	memcpy(&matWorld.m[3][0], &m_pState->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	// m_pSphere->Render(&matWorld);
	m_pStaticMesh->RenderMesh(&m_pState->m_matWorld);
}

void CGreenCube::Release(void)
{

}