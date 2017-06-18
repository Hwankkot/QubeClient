#include "stdafx.h"
#include "RedCube.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "RayCollision.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Value.h"

CRedCube::CRedCube(LPDIRECT3DDEVICE9 pDevice)
: CCube(pDevice)
, m_iMovedCount(0)
, m_IsActived(false)
, m_IsDeactive(false)
{

}

CRedCube::~CRedCube(void)
{
	Release();
}

CRedCube* CRedCube::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData)
{
	CRedCube* pGameObject = new CRedCube(pDevice);

	if(FAILED(pGameObject->InitCube(pCubeData)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}   

HRESULT CRedCube::InitCube(const Engine::CUBEDATA* pCubeData)
{
	m_pInput = Engine::Get_Input();
	m_pResourceMgr = Engine::Get_ResourceMgr();

	FAILED_CHECK(AddComponent());

	float fCubeScale = CCube::CUBESCALE;
	
	if(pCubeData)
	{
		m_dwCubeType = pCubeData->dwCubeType;
		memcpy(m_pState->m_fAngle, &pCubeData->fAngle, sizeof(float) * 3);
		m_pState->m_vScale = D3DXVECTOR3(fCubeScale, fCubeScale, fCubeScale);
		m_pState->m_vDir = pCubeData->vDir;
		m_pState->m_vPos = pCubeData->vPos;
		m_pState->m_matWorld = pCubeData->matWorld;
	}
	
	m_vOriginPos = m_pState->m_vPos;

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pStaticMesh->GetMinMax(&StdVtx3);

	D3DXVECTOR3 vCenter = (StdVtx3._min + StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (StdVtx3._max + D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pState->m_matWorld);

	m_pState->m_vDir = vTopCenter - vCenter;

	D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);

	m_fDistance = CCube::CUBERADIUS * 2.f;
	m_vMaxPos = m_vOriginPos + (m_pState->m_vDir * m_fDistance * 3.f);
	
	m_pState->Update();
	Compute_ViewZ(m_pState->m_vPos);
	return S_OK;
}

HRESULT CRedCube::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_RED);
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

void CRedCube::Update(void)
{ 
	if(!m_IsUpdate)
		return;

	MovingCube();

	Engine::CGameObject::Update();
	Compute_ViewZ(m_pState->m_vPos);
}

void CRedCube::Render(void)
{
	if(!m_IsUpdate)
		return;

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	memcpy(&matWorld.m[3][0], &m_pState->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	m_pStaticMesh->RenderMesh(&m_pState->m_matWorld);
}

void CRedCube::Release(void)
{	
	
}

void CRedCube::ActiveCube(void)
{
	if(m_IsActived || m_IsDeactive)
		return;

	if(m_iMovedCount >= 3)
	{
		if( m_vMaxPos != m_pState->m_vPos)
		{
			m_pState->m_vPos = m_vMaxPos;
			m_iMovedCount = 2;
		}
		return;

		m_IsActived = false;
	}
	
	m_IsActived = true;

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pStaticMesh->GetMinMax(&StdVtx3);

	D3DXVECTOR3 vCenter = (StdVtx3._min + StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (StdVtx3._max + D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pState->m_matWorld);

	m_pState->m_vDir = vTopCenter - vCenter;

	D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);

	m_vMovedPos = m_pState->m_vPos + (m_pState->m_vDir * m_fDistance);
	
	++m_iMovedCount;
}

void CRedCube::DeactiveCube(void)
{
	if(m_IsActived || m_IsDeactive)
		return;

	if(m_iMovedCount <= 0)
	{
		if(m_vOriginPos != m_pState->m_vPos)
		{
			m_pState->m_vPos = m_vOriginPos;
			m_iMovedCount = 0;
		}
		return;

		m_IsDeactive = false;
	}

	m_IsDeactive = true;

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pStaticMesh->GetMinMax(&StdVtx3);

	D3DXVECTOR3 vCenter = (StdVtx3._min + StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (StdVtx3._max + D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pState->m_matWorld);

	m_pState->m_vDir = vTopCenter - vCenter;

	D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);

	m_vMovedPos = m_pState->m_vPos + (m_pState->m_vDir * m_fDistance * -1.f);
	
	--m_iMovedCount;
}

void CRedCube::MovingCube(void) 
{
	if(m_IsActived)
	{
		m_pState->m_vPos += (m_pState->m_vDir * Engine::Get_TimeMgr()->GetTime() * 7.f);

		if( D3DXVec3Length(&D3DXVECTOR3(m_vMovedPos - m_pState->m_vPos)) < 0.1f)
		{
			m_pState->m_vPos = m_vMovedPos;
			m_IsActived = false;
		}
	}

	if(m_IsDeactive)
	{
		m_pState->m_vPos += (m_pState->m_vDir * Engine::Get_TimeMgr()->GetTime() * 7.f  * -1.f);

		if( D3DXVec3Length(&D3DXVECTOR3(m_vMovedPos - m_pState->m_vPos)) < 0.1f)
		{
			m_pState->m_vPos = m_vMovedPos;
			m_IsDeactive = false;
		}
	}
}