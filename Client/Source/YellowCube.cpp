#include "stdafx.h"
#include "YellowCube.h"

#include "CollisionManager.h"
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

CYellowCube::CYellowCube(LPDIRECT3DDEVICE9 pDevice)
: CCube(pDevice)
, m_IsActived(false)
, m_IsDeactive(false)
, m_pCollisionMgr(NULL)
{

}

CYellowCube::~CYellowCube(void)
{
	Release();
}

CYellowCube* CYellowCube::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData)
{
	CYellowCube* pGameObject = new CYellowCube(pDevice);

	if(FAILED(pGameObject->InitCube(pCubeData)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}   

HRESULT CYellowCube::InitCube(const Engine::CUBEDATA* pCubeData)
{
	m_pInput = Engine::Get_Input();
	m_pResourceMgr = Engine::Get_ResourceMgr();
	m_pCollisionMgr = CCollisionManager::GetInstance();
	
	FAILED_CHECK(AddComponent());
	
	// 초기화
	float fCubeScale = CCube::CUBESCALE;
	if(pCubeData)
	{
		for(int i = 0; i < POS_END; ++i)
		{
			m_dwCubeType = pCubeData->dwCubeType;
			memcpy(m_pStateArray[i]->m_fAngle, &pCubeData->fAngle, sizeof(float) * 3);
			m_pStateArray[i]->m_vScale = D3DXVECTOR3(fCubeScale, fCubeScale, fCubeScale);
			m_pStateArray[i]->m_vDir = pCubeData->vDir;
			m_pStateArray[i]->m_vPos = pCubeData->vPos;
			m_pStateArray[i]->m_matWorld = pCubeData->matWorld;
		}
	}

	for(int i = 0; i < POS_END; ++i)
		m_pStateArray[i]->Update();
	
	Compute_ViewZ(m_pStateArray[POS_MID]->m_vPos);

	m_fDistance = CCube::CUBERADIUS * 2.f;

	m_pStaticMesh[POS_MID]->GetMinMax(&m_RevisedStdVtx3);

	m_StdVtx3 = m_RevisedStdVtx3;
	m_RevisedStdVtx3._min.x *= 2.f;
	m_RevisedStdVtx3._max.x *= 2.f;


	// MID
	D3DXVECTOR3 vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_MID]->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pStateArray[POS_MID]->m_matWorld);

	m_pStateArray[POS_MID]->m_vDir = vTopCenter - vCenter;
	D3DXVec3Normalize(&m_pStateArray[POS_MID]->m_vDir, &m_pStateArray[POS_MID]->m_vDir);
	m_vOriginPos[POS_MID] = m_pStateArray[POS_MID]->m_vPos;
	m_vMaxPos[POS_MID] = m_pStateArray[POS_MID]->m_vPos + m_pStateArray[POS_MID]->m_vDir * m_fDistance * 3.f;



	// LEFT
	vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vLeft = (m_StdVtx3._min + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._max.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_LEFT]->m_matWorld);
	D3DXVec3TransformCoord(&vLeft, &vLeft, &m_pStateArray[POS_LEFT]->m_matWorld);

	m_pStateArray[POS_LEFT]->m_vDir = vLeft - vCenter;
	D3DXVec3Normalize(&m_pStateArray[POS_LEFT]->m_vDir, &m_pStateArray[POS_LEFT]->m_vDir);
	m_vOriginPos[POS_LEFT] = m_pStateArray[POS_LEFT]->m_vPos += m_pStateArray[POS_LEFT]->m_vDir * m_fDistance;
	m_vMaxPos[POS_LEFT] = m_pStateArray[POS_LEFT]->m_vPos + m_pStateArray[POS_MID]->m_vDir * m_fDistance * 3.f;
	

	// RIGHT
	vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vPOS_RIGHT = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._max.x, m_StdVtx3._min.y, m_StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_RIGHT]->m_matWorld);
	D3DXVec3TransformCoord(&vPOS_RIGHT, &vPOS_RIGHT, &m_pStateArray[POS_RIGHT]->m_matWorld);

	m_pStateArray[POS_RIGHT]->m_vDir = vPOS_RIGHT - vCenter;
	D3DXVec3Normalize(&m_pStateArray[POS_RIGHT]->m_vDir, &m_pStateArray[POS_RIGHT]->m_vDir);
	m_vOriginPos[POS_RIGHT] = m_pStateArray[POS_RIGHT]->m_vPos += m_pStateArray[POS_RIGHT]->m_vDir * m_fDistance;
	m_vMaxPos[POS_RIGHT] = m_pStateArray[POS_RIGHT]->m_vPos + m_pStateArray[POS_MID]->m_vDir * m_fDistance * 3.f;


	for(int i = 0; i < POS_END; ++i)
	{
		m_pStateArray[i]->m_vDir = m_pStateArray[POS_MID]->m_vDir;
		m_pStateArray[i]->Update();

		m_pArrStaticMesh[i] = m_pStaticMesh[i]->GetMesh(); 
	}

	

	ZeroMemory(m_IsSelected, sizeof(bool) * POS_END);

	return S_OK;
}

HRESULT CYellowCube::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pStateArray[POS_LEFT] = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORMPOS_LEFT, pComponent));

	//Transform
	pComponent = m_pStateArray[POS_MID] = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRNASFORMPOS_MID, pComponent));

	//Transform
	pComponent = m_pStateArray[POS_RIGHT] = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORMPOS_RIGHT, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_YELLOW);
	m_pStaticMesh[POS_LEFT] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATICPOS_LEFT, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_YELLOW);
	m_pStaticMesh[POS_MID] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATICPOS_MID, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_YELLOW);
	m_pStaticMesh[POS_RIGHT] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(MESH_STATICPOS_RIGHT, pComponent));

	return S_OK;
} 

void CYellowCube::Update(void)
{  
	if(!m_IsUpdate)
		return;

	if(m_IsActived)
	{		
		BOOL IsEnd = 0;

		for(int i = 0; i < POS_END; ++i)
		{
			m_pStateArray[i]->m_vPos += (m_pStateArray[i]->m_vDir * Engine::Get_TimeMgr()->GetTime() * 7.f);

			if(D3DXVec3Length(&D3DXVECTOR3(m_vMovedPos[i] - m_pStateArray[i]->m_vPos)) < 0.2f)
			{
				m_pStateArray[i]->m_vPos = m_vMovedPos[i];
				m_IsSelected[i] = false;
				++IsEnd;
			}
		}

		if(IsEnd == 3)
		{
			m_IsActived = false;
		}
	}

	if(m_IsDeactive)
	{
		BOOL IsEnd = 0;

		for(int i = 0; i < POS_END; ++i)
		{
			m_pStateArray[i]->m_vPos += (m_pStateArray[i]->m_vDir * Engine::Get_TimeMgr()->GetTime() * 7.f * -1.f);

			if(D3DXVec3Length(&D3DXVECTOR3(m_vMovedPos[i] - m_pStateArray[i]->m_vPos)) < 0.2f)
			{
				m_pStateArray[i]->m_vPos = m_vOriginPos[i];
				m_IsSelected[i] = false;
				++IsEnd;
			}
		}

		if(IsEnd == 3)
			m_IsDeactive = false;
	}

	Engine::CGameObject::Update();
	Compute_ViewZ(m_pStateArray[POS_MID]->m_vPos);
}

void CYellowCube::Render(void)
{
	if(!m_IsUpdate)
		return;

	for(int i = 0; i < POS_END; ++i)
	{
		m_pStaticMesh[i]->RenderMesh(&m_pStateArray[i]->m_matWorld); 
	}
}

void CYellowCube::Release(void)
{
	

}	

void CYellowCube::GetMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3)
{
	*pStdVtx3 = m_RevisedStdVtx3;
}

void CYellowCube::GetChildMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3)
{
	*pStdVtx3 = m_StdVtx3;
}

const Engine::CTransform* const CYellowCube::GetCubeData(void) const
{
	return m_pStateArray[POS_MID];
}

LPD3DXMESH* CYellowCube::GetMeshArray(void)
{
	return m_pArrStaticMesh;
}

void CYellowCube::ActiveCube(void)
{
	if(m_IsActived || m_IsDeactive)
		return;

	for(int i = 0; i < POS_END; ++i)
	{
		BOOL IsEnd = 0;
		
		if(m_IsSelected[i])
		{
			m_vMovedPos[i] = m_pStateArray[i]->m_vPos;
			++IsEnd;
			m_IsActived = false;
		}

		if(IsEnd > 0)
			return ;
	}

	// Get Ray
	Engine::RAY3 Ray3;
	D3DXVECTOR2 vMousePos(float(WINCX >> 1), float(WINCY >> 1));

	D3DXMATRIX matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3 vSrc;
	vSrc.x = (  vMousePos.x / (WINCX >> 1) - 1.f) / matProj._11;
	vSrc.y = ( -vMousePos.y / (WINCY >> 1) + 1.f) / matProj._22;
	vSrc.z = 1.f;

	Ray3.pos = D3DXVECTOR3(0.f, 0.f, 0.f);
	Ray3.dir = vSrc - Ray3.pos;
	D3DXVec3Normalize(&Ray3.dir ,&Ray3.dir);

	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matView);
	D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matView);

	// 남은 Cube들과 충돌 처리를 함
	
	int i = 0;
	for(; i < POS_END; ++i)
	{
		Engine::RAY3 ConvertedRay3 = Ray3;
	
		D3DXMATRIX matInvWorld;
		D3DXMatrixInverse(&matInvWorld, NULL, &m_pStateArray[i]->m_matWorld);

		D3DXVec3TransformCoord(&ConvertedRay3.pos, &ConvertedRay3.pos, &matInvWorld);
		D3DXVec3TransformNormal(&ConvertedRay3.dir, &ConvertedRay3.dir, &matInvWorld);

		if(CCollisionManager::GetInstance()->IsRayIntersectedBox(m_StdVtx3, ConvertedRay3, NULL, NULL))
			break;
	}

	m_IsSelected[i] = true;

 

	// 처리 시작
	if(m_IsSelected[POS_LEFT])
	{
		D3DXVECTOR3 vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
		D3DXVECTOR3 vTopCenter = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._min.z)) * 0.5f;

		D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_LEFT]->m_matWorld);
		D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pStateArray[POS_LEFT]->m_matWorld);

		D3DXVECTOR3 vDir;

		vDir = vTopCenter - vCenter;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pStateArray[POS_LEFT]->m_vDir = m_pStateArray[POS_MID]->m_vDir = m_pStateArray[POS_RIGHT]->m_vDir = vDir;

		m_vMovedPos[POS_LEFT] = m_vMaxPos[POS_LEFT];
		m_vMovedPos[POS_MID] = m_vOriginPos[POS_MID] + (vDir * m_fDistance * 2.f);
		m_vMovedPos[POS_RIGHT] = m_vOriginPos[POS_RIGHT] + (vDir * m_fDistance * 1.f);

	}

	else if(m_IsSelected[POS_MID])
	{
		D3DXVECTOR3 vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
		D3DXVECTOR3 vTopCenter = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._min.z)) * 0.5f;

		D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_LEFT]->m_matWorld);
		D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pStateArray[POS_LEFT]->m_matWorld);

		D3DXVECTOR3 vDir;

		vDir = vTopCenter - vCenter;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pStateArray[POS_LEFT]->m_vDir = m_pStateArray[POS_MID]->m_vDir = m_pStateArray[POS_RIGHT]->m_vDir = vDir;

		m_vMovedPos[POS_LEFT] = m_vOriginPos[POS_LEFT] + (vDir * m_fDistance * 1.f);
		m_vMovedPos[POS_MID] = m_vOriginPos[POS_MID] + (vDir * m_fDistance * 2.f);
		m_vMovedPos[POS_RIGHT] = m_vOriginPos[POS_RIGHT] + (vDir * m_fDistance * 1.f);

	}

	else if(m_IsSelected[POS_RIGHT])
	{
		D3DXVECTOR3 vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
		D3DXVECTOR3 vTopCenter = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._min.z)) * 0.5f;

		D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_LEFT]->m_matWorld);
		D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pStateArray[POS_LEFT]->m_matWorld);

		D3DXVECTOR3 vDir;

		vDir = vTopCenter - vCenter;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pStateArray[POS_LEFT]->m_vDir = m_pStateArray[POS_MID]->m_vDir = m_pStateArray[POS_RIGHT]->m_vDir = vDir;

		m_vMovedPos[POS_LEFT] = m_vOriginPos[POS_LEFT] + (vDir * m_fDistance * 1.f);
		m_vMovedPos[POS_MID] = m_vOriginPos[POS_MID] + (vDir * m_fDistance * 2.f);
		m_vMovedPos[POS_RIGHT] = m_vMaxPos[POS_RIGHT];
	}

	m_IsActived = true;
}


void CYellowCube::DeactiveCube(void)
{
	if(m_IsActived || m_IsDeactive)
		return;

	for(int i = 0; i < POS_END; ++i)
	{
		BOOL IsEnd = 0;

		if(m_vOriginPos[i] == m_pStateArray[i]->m_vPos)
		{
			++IsEnd;
			m_IsDeactive = false;
		}

		if(IsEnd > 0)
			return ;
	}

	D3DXVECTOR3 vCenter = (m_StdVtx3._min + m_StdVtx3._max) * 0.5f;
	D3DXVECTOR3 vTopCenter = (m_StdVtx3._max + D3DXVECTOR3(m_StdVtx3._min.x, m_StdVtx3._max.y, m_StdVtx3._min.z)) * 0.5f;

	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pStateArray[POS_MID]->m_matWorld);
	D3DXVec3TransformCoord(&vTopCenter, &vTopCenter, &m_pStateArray[POS_MID]->m_matWorld);

	D3DXVECTOR3 vDir;

	vDir = vTopCenter - vCenter;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pStateArray[POS_LEFT]->m_vDir = m_pStateArray[POS_MID]->m_vDir = m_pStateArray[POS_RIGHT]->m_vDir = vDir;

	for(int i = 0 ; i < POS_END; ++i)
		m_vMovedPos[i] = m_vOriginPos[i];

	m_IsDeactive = true;
}

Engine::CTransform** CYellowCube::GetCubeArray(void) 
{
	return m_pStateArray;
}

