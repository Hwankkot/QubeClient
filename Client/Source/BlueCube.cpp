#include "stdafx.h"
#include "BlueCube.h"

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

CBlueCube::CBlueCube(LPDIRECT3DDEVICE9 pDevice)
: CCube(pDevice)
, m_IsActived(false)
, m_IsDeactive(false)
, m_IsCharged(false)
, m_IsDisCharged(false)
{

}

CBlueCube::~CBlueCube(void)
{
	Release();
}

CBlueCube* CBlueCube::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData)
{
	CBlueCube* pGameObject = new CBlueCube(pDevice);

	if(FAILED(pGameObject->InitCube(pCubeData)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}   

HRESULT CBlueCube::InitCube(const Engine::CUBEDATA* pCubeData)
{
	m_pInput = Engine::Get_Input();
	m_pResourceMgr = Engine::Get_ResourceMgr();

	FAILED_CHECK(AddComponent());

	if(pCubeData)
	{
		m_dwCubeType = pCubeData->dwCubeType;
		memcpy(m_pState->m_fAngle, &pCubeData->fAngle, sizeof(float) * 3);
		m_pState->m_vScale = D3DXVECTOR3(CCube::CUBESCALE, CCube::CUBESCALE * 0.6f, CCube::CUBESCALE);
		m_pState->m_vDir = pCubeData->vDir;
		m_pState->m_vPos = pCubeData->vPos;
		m_pState->m_matWorld = pCubeData->matWorld;
	}
	
	m_pState->Update();
	Compute_ViewZ(m_pState->m_vPos);
	m_vOriginPos = m_pState->m_vPos;

	return S_OK;
}

HRESULT CBlueCube::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_BLUE);
	m_pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATIC, pComponent));

	return S_OK;
} 

void CBlueCube::Update(void)
{  
	if(!m_IsUpdate)
		return;

	MovingCube();

	Engine::CGameObject::Update();
	Compute_ViewZ(m_pState->m_vPos);
}

void CBlueCube::Render(void)
{
	if(!m_IsUpdate)
		return;

	m_pStaticMesh->RenderMesh(&m_pState->m_matWorld);
}


void CBlueCube::Release(void)
{

}

void CBlueCube::ActiveCube(void)
{
	if(m_IsActived || m_IsDeactive)
		return;

	if(D3DXVec3Length(&D3DXVECTOR3(m_vMovedPos - m_pState->m_vPos)) == 0.f)
	{
		if(m_IsActived)
			m_IsActived = false;

		return ;
	}

	m_IsActived = true;

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pStaticMesh->GetMinMax(&StdVtx3);

	D3DXVECTOR3 vCenter = (StdVtx3._min + StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (StdVtx3._max + D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pState->m_matWorld);

	m_pState->m_vDir = vTopCenter - vCenter;
	
	m_vMovedPos = m_pState->m_vPos + (m_pState->m_vDir * -1.f);

	D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);
}

void CBlueCube::DeactiveCube(void)
{
	if(m_IsActived || m_IsDeactive)
		return;

	if(D3DXVec3Length(&D3DXVECTOR3(m_vOriginPos - m_pState->m_vPos)) == 0.f)
	{
		if(m_IsDeactive)
			m_IsDeactive = false;

		return ;
	}

	m_IsDeactive = true;
	m_IsCharged = false;

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pStaticMesh->GetMinMax(&StdVtx3);

	D3DXVECTOR3 vCenter = (StdVtx3._min + StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (StdVtx3._max + D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pState->m_matWorld);

	m_pState->m_vDir = vTopCenter - vCenter;

	D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);
}

void CBlueCube::MovingCube(void)
{
	if(m_IsActived)
	{
		m_pState->m_vPos += (m_pState->m_vDir * Engine::Get_TimeMgr()->GetTime() * 7.f * -1.f);

		if( D3DXVec3Length(&D3DXVECTOR3(m_vMovedPos - m_pState->m_vPos)) < 0.1f)
		{
			
			m_pState->m_vPos = m_vMovedPos;
			m_IsActived = false;
			m_IsCharged = true;
		}
	}

	if(m_IsDeactive)
	{
		m_pState->m_vPos += (m_pState->m_vDir * Engine::Get_TimeMgr()->GetTime() * 7.f);

		if( D3DXVec3Length(&D3DXVECTOR3(m_vOriginPos - m_pState->m_vPos)) < 0.1f)
		{
			m_pState->m_vPos = m_vOriginPos;
			m_IsDeactive = false;
		}
	}

	if(m_IsDisCharged)
	{
		m_pState->m_vPos += (m_pState->m_vDir * Engine::Get_TimeMgr()->GetTime() * 14.f);

		if( D3DXVec3Length(&D3DXVECTOR3(m_vOriginPos - m_pState->m_vPos)) < 0.1f)
		{
			m_pState->m_vPos = m_vOriginPos;
			m_IsDisCharged = false;
		}
	}
}

void CBlueCube::Discharge(void)
{
	m_IsDeactive = true;
	m_IsActived = false;
	m_IsCharged = false;

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pStaticMesh->GetMinMax(&StdVtx3);

	D3DXVECTOR3 vCenter = (StdVtx3._min + StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (StdVtx3._max + D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pState->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pState->m_matWorld);

 	m_pState->m_vDir = vTopCenter - vCenter;

	D3DXVec3Normalize(&m_pState->m_vDir, &m_pState->m_vDir);
}

