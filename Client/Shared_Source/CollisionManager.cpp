#include "stdafx.h"
#include "CollisionManager.h"

#include "Cube.h"
#include "Enum.h"
#include "Engine_Struct.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Transform.h"
#include "TextureParticleManager.h"
#include "Value.h"
#include "SoundMgr.h"

#include "BlueCube.h"
#include "YellowCube.h"
IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager(void)
: m_pQubelistMap(NULL)
, m_pDevice(Engine::Get_GraphicDev()->GetDevice())
, m_pTextureParticlManager(CTextureParticleManager::GetInstance())
, m_IsCreatedParticle(false)
{

}

CCollisionManager::~CCollisionManager(void)
{
	CTextureParticleManager::GetInstance()->DestroyInstance();
}

void CCollisionManager::AddQubeObjectMap(std::map<DWORD, std::vector<Engine::CGameObject*>>* pQubeObjectlistMap)
{
	m_pQubelistMap = pQubeObjectlistMap;
	m_QubelistMap = *(m_pQubelistMap);
}

void CCollisionManager::RemoveQubeObjectMap(void)
{
	m_pQubelistMap = NULL;
	m_QubelistMap.clear();
}

void CCollisionManager::Update(void)
{
	UpdateCullingQube();
	m_pTextureParticlManager->Update();
}

bool Compare_ViewZ(Engine::CGameObject* pSour, Engine::CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

void CCollisionManager::UpdateCullingQube(void)
{
	// 충돌 처리할 리스트들을 선별한다.
	// Frustum 으로 선별 후
	// Z버퍼로 순서 정렬 

	for(int i = GAMEOBJECTID::CUBE_WHITE; i  < GAMEOBJECTID::CUBE_END; ++i)
	{
		sort(m_QubelistMap[i].begin(), m_QubelistMap[i].end(), Compare_ViewZ);
	}
}

bool CCollisionManager::IsRayIntersectedSphere(const Engine::RAY3& Ray3, const D3DXVECTOR3& vSphereCenter, const float& fRadius, D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 vRaytoSphere = vSphereCenter - Ray3.pos;

	float fAdjacentSide = D3DXVec3Dot(&vRaytoSphere, &Ray3.dir);

	float fRaytoSphereLength = D3DXVec3Length(&vRaytoSphere);

	if(fAdjacentSide < 0 && fRaytoSphereLength > fRadius)
		return false;

	float fOppositeSide_2Squared = (fRaytoSphereLength * fRaytoSphereLength) - (fAdjacentSide * fAdjacentSide);

	float fRadius_2Squared = fRadius * fRadius;

	if(fOppositeSide_2Squared > fRadius_2Squared)
		return false;

	return true;
}

bool CCollisionManager::IsRayIntersectedBox(const Engine::STANDARD_VERTEX3& StdVtx3, const Engine::RAY3& Ray3, D3DXVECTOR3* pPos, WORD* pEdge)
{
	bool IsIntersected = false;
	float fU, fV, fDist, fDistTmp = FLOATMAX;
	D3DXVECTOR3 vPos(0.f, 0.f, 0.f);
	WORD wEdge(6);

	D3DXVECTOR3 aVertex[8] = { D3DXVECTOR3(0.f, 0.f, 0.f) };

	aVertex[0] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z);
	aVertex[1] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._max.y, StdVtx3._min.z);
	aVertex[2] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._min.y, StdVtx3._min.z);
	aVertex[3] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._min.y, StdVtx3._min.z);
	aVertex[4] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._max.z);
	aVertex[5] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._max.y, StdVtx3._max.z);
	aVertex[6] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._min.y, StdVtx3._max.z);
	aVertex[7] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._min.y, StdVtx3._max.z);


	// TOP 0 , BOT 1, RIGHT 2, LEFT 3, FRONT 4, BACK 5


	// Top 0

	// Right - Up
	if(D3DXIntersectTri(&aVertex[5], &aVertex[4], &aVertex[1], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[5] + (aVertex[4] - aVertex[5]) * fU + (aVertex[1] - aVertex[5]) * fV;
			wEdge = 0;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[0], &aVertex[1], &aVertex[4], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[0] + (aVertex[1] - aVertex[0]) * fU + (aVertex[4] - aVertex[0]) * fV;
			wEdge = 0;
			IsIntersected = true;
		}
	}

	// Bot 1

	// Right - Up
	if(D3DXIntersectTri(&aVertex[6], &aVertex[7], &aVertex[2], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[6] + (aVertex[7] - aVertex[6]) * fU + (aVertex[2] - aVertex[6]) * fV;
			wEdge = 1;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[3], &aVertex[2], &aVertex[3], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[3] + (aVertex[2] - aVertex[3]) * fU + (aVertex[7] - aVertex[3]) * fV;
			wEdge = 1;
			IsIntersected = true;
		}
	}

	// Left 2

	// Right - Up
	if(D3DXIntersectTri(&aVertex[0], &aVertex[4], &aVertex[3], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[0] + (aVertex[4] - aVertex[0]) * fU + (aVertex[3] - aVertex[0]) * fV;
			wEdge = 2;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[7], &aVertex[3], &aVertex[4], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[7] + (aVertex[3] - aVertex[7]) * fU + (aVertex[4] - aVertex[7]) * fV;
			wEdge = 2;
			IsIntersected = true;
		}
	}

	// RIGHT 3

	// Right - Up
	if(D3DXIntersectTri(&aVertex[5], &aVertex[1], &aVertex[6], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[5] + (aVertex[1] - aVertex[5]) * fU + (aVertex[6] - aVertex[5]) * fV;
			wEdge = 3;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[2], &aVertex[6], &aVertex[1], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[2] + (aVertex[6] - aVertex[2]) * fU + (aVertex[1] - aVertex[2]) * fV;
			wEdge = 3;
			IsIntersected = true;
		}
	}

	// Front 4

	// Right - Up
	if(D3DXIntersectTri(&aVertex[1], &aVertex[0], &aVertex[2], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[1] + (aVertex[0] - aVertex[1]) * fU + (aVertex[2] - aVertex[1]) * fV;
			wEdge = 4;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[3], &aVertex[2], &aVertex[0], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[3] + (aVertex[2] - aVertex[3]) * fU + (aVertex[0] - aVertex[3]) * fV;
			wEdge = 4;
			IsIntersected = true;
		}
	}

	// Back 5

	// Right - Up
	if(D3DXIntersectTri(&aVertex[5], &aVertex[4], &aVertex[6], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[5] + (aVertex[4] - aVertex[5]) * fU + (aVertex[6] - aVertex[5]) * fV;
			wEdge = 5;
			IsIntersected = true;
		} 
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[7], &aVertex[6], &aVertex[4], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[7] + (aVertex[6] - aVertex[7]) * fU + (aVertex[4] - aVertex[7]) * fV;
			wEdge = 5;
			IsIntersected = true;
		}
	}

	if(pPos != NULL)
		*pPos = vPos;

	if(pEdge != NULL)
		*pEdge = wEdge;

	return IsIntersected;
}

bool CCollisionManager::IsRayIntersectedMeshtoReviseHeight(D3DXVECTOR3* pPos)
{	
	 OBJECTLISTMAP& ObjectlistMap = m_QubelistMap;

	 D3DXVECTOR3 vPos = *pPos;

	 UpdateCullingQube();

	
	 for(int i = GAMEOBJECTID::CUBE_WHITE; i != GAMEOBJECTID::CUBE_END; ++i)
	 {
		if(i == GAMEOBJECTID::CUBE_BLUE)
			continue;

		if(i == GAMEOBJECTID::CUBE_YELLOW)
			continue;

		if(ObjectlistMap.count(i) == NULL)
			continue;

		OBJECTLIST::iterator iterlist = ObjectlistMap[i].begin();
		OBJECTLIST::iterator iterlist_end = ObjectlistMap[i].end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			if((*iterlist)->Get_ViewZ() < 32.f)
			{
				CCube* pCube = static_cast<CCube*>((*iterlist));
							
				D3DXMATRIX matWorld = pCube->GetCubeData()->m_matWorld;
				D3DXMATRIX matInvWorld;

				 vPos = *pPos;

				 vPos.y += 10.f;

				D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);
				D3DXVec3TransformCoord(&vPos, &vPos, &matInvWorld);

				BOOL IsIntersectedUp = false;
				BOOL IsIntersectedDown = false;
				
				D3DXIntersect(pCube->GetMesh(), &vPos, &g_vUp, &IsIntersectedUp, NULL, NULL, NULL, NULL, NULL, NULL);
				D3DXIntersect(pCube->GetMesh(), &vPos, &g_vDown, &IsIntersectedDown, NULL, NULL, NULL, NULL, NULL, NULL);

				if(IsIntersectedDown && !IsIntersectedUp)
				{
					Engine::STANDARD_VERTEX3 StdVtx3;
  					pCube->GetMinMax(&StdVtx3);

					D3DXVec3TransformCoord(&StdVtx3._max, &StdVtx3._max, &matWorld);

					 if( StdVtx3._max.y < 0.f)
						  StdVtx3._max.y *= -1.f;
 					pPos->y = StdVtx3._max.y + JOHNHEIGHT;

					return true;
				}
			}
		}

		if(ObjectlistMap.count(GAMEOBJECTID::CUBE_YELLOW) == NULL)
			continue;

		iterlist = ObjectlistMap[GAMEOBJECTID::CUBE_YELLOW].begin();
		iterlist_end = ObjectlistMap[GAMEOBJECTID::CUBE_YELLOW].end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			if((*iterlist)->Get_ViewZ() < 32.f)
			{
				CYellowCube* pCube = static_cast<CYellowCube*>((*iterlist));
				
				for(int i = 0; i < 3; ++i)
				{
					D3DXMATRIX matWorld = (*(pCube->GetCubeArray() + i))->m_matWorld;
					D3DXMATRIX matInvWorld;

					BOOL IsIntersectedUp = false;
					BOOL IsIntersectedDown = false;
					
					D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);
					D3DXVec3TransformCoord(&vPos, pPos, &matInvWorld);

					LPD3DXMESH* ppMeshStorage = pCube->GetMeshArray();
					D3DXIntersect(*(ppMeshStorage + i), &vPos, &g_vUp, &IsIntersectedUp, NULL, NULL, NULL, NULL, NULL, NULL);
					D3DXIntersect(*(ppMeshStorage + i), &vPos, &g_vDown, &IsIntersectedDown, NULL, NULL, NULL, NULL, NULL, NULL);
					
					if(IsIntersectedDown && !IsIntersectedUp)
					{
						Engine::STANDARD_VERTEX3 StdVtx3;
						pCube->GetChildMinMax(&StdVtx3);

						D3DXVec3TransformCoord(&StdVtx3._max, &StdVtx3._max, &matWorld);
						pPos->y = StdVtx3._max.y + JOHNHEIGHT - 4.f;
						
						return true;
					}
					else if(IsIntersectedDown && IsIntersectedUp)
					{
						Engine::STANDARD_VERTEX3 StdVtx3;
						pCube->GetChildMinMax(&StdVtx3);

						D3DXVec3TransformCoord(&StdVtx3._max, &StdVtx3._max, &matWorld);
						pPos->y = StdVtx3._max.y + JOHNHEIGHT;

					}
				}
			}
		}
	 }
	return false;
}

void CCollisionManager::CollisionActiveCube(void)
{

	CSoundMgr::GetInstance()->PlaySkillSound(L"Blue_Cube_Up.ogg");
	// 나중에BLUE->RED->YELLOW- 순으로 우선 Pikcing 하도록 오브젝트 ID 다시 만들 것.

	Engine::RAY3 OriginRay3;

	GetRay(&OriginRay3);

	OBJECTLISTMAP& QubelistMap = m_QubelistMap;

	for(int i = GAMEOBJECTID::CUBE_WHITE; i != GAMEOBJECTID::CUBE_END; ++i)
	{
		if(i == GAMEOBJECTID::CUBE_YELLOW)
			continue;

		OBJECTLIST::iterator iterlist = QubelistMap[i].begin();
		OBJECTLIST::iterator iterlist_end = QubelistMap[i].end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			Engine::STANDARD_VERTEX3 StdVtx3;
			((CCube*)(*iterlist))->GetMinMax(&StdVtx3);

			Engine::RAY3 Ray3 = OriginRay3;

			D3DXMATRIX matInvWorld;
			D3DXMatrixInverse(&matInvWorld, NULL, &((CCube*)(*iterlist))->GetCubeData()->m_matWorld);

			D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
			D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

			D3DXVECTOR3 vPos;
			if(IsRayIntersectedBox(StdVtx3, Ray3, &vPos, NULL))
			{				
				((CCube*)(*iterlist))->ActiveCube();

				D3DXVec3TransformCoord(&vPos, &vPos, &((CCube*)(*iterlist))->GetCubeData()->m_matWorld);
				m_pTextureParticlManager->AddParticle(vPos);
			}
		}
	}


	if(QubelistMap.count(GAMEOBJECTID::CUBE_YELLOW) == NULL)
		return;

	OBJECTLIST::iterator iterlist = QubelistMap[GAMEOBJECTID::CUBE_YELLOW].begin();
	OBJECTLIST::iterator iterlist_end = QubelistMap[GAMEOBJECTID::CUBE_YELLOW].end();

	for(; iterlist != iterlist_end; ++iterlist)
	{
		Engine::STANDARD_VERTEX3 StdVtx3;
		CYellowCube* pCube = static_cast<CYellowCube*>((*iterlist));

		pCube->GetChildMinMax(&StdVtx3);

		for(int i = 0; i < 3; ++i)
		{
			Engine::RAY3 Ray3 = OriginRay3;

			D3DXMATRIX matWorld = (*(pCube->GetCubeArray() + i))->m_matWorld;
			D3DXMATRIX matInvWorld;
			D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

			D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
			D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

			D3DXVECTOR3 vPos;
			if(IsRayIntersectedBox(StdVtx3, Ray3, &vPos, NULL))
			{				
				pCube->ActiveCube();

				D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);
				m_pTextureParticlManager->AddParticle(vPos);

				return;
			}
		}
	}
}

void CCollisionManager::CollisionDeactiveCube(void)
{

	CSoundMgr::GetInstance()->PlaySkillSound(L"Blue_Cube_down.ogg");
	Engine::RAY3 OriginRay3;

	GetRay(&OriginRay3);

	OBJECTLISTMAP& QubelistMap = m_QubelistMap;

	for(int i = GAMEOBJECTID::CUBE_BLUE; i != GAMEOBJECTID::CUBE_GRAY; ++i)
	{
		if(i == GAMEOBJECTID::CUBE_YELLOW)
			continue;

		OBJECTLIST::iterator iterlist = QubelistMap[i].begin();
		OBJECTLIST::iterator iterlist_end = QubelistMap[i].end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			Engine::STANDARD_VERTEX3 StdVtx3;
			((CCube*)(*iterlist))->GetMinMax(&StdVtx3);

			Engine::RAY3 Ray3 = OriginRay3;

			D3DXMATRIX matInvWorld;
			D3DXMatrixInverse(&matInvWorld, NULL, &((CCube*)(*iterlist))->GetCubeData()->m_matWorld);

			D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
			D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

			D3DXVECTOR3 vPos;

			if(IsRayIntersectedBox(StdVtx3, Ray3, &vPos, NULL))
			{
				((CCube*)(*iterlist))->DeactiveCube();
				
				D3DXVec3TransformCoord(&vPos, &vPos, &((CCube*)(*iterlist))->GetCubeData()->m_matWorld);
				m_pTextureParticlManager->AddParticle(vPos);

				return;
			}
		}
	}

	if(QubelistMap.count(GAMEOBJECTID::CUBE_YELLOW) == NULL)
		return;

	OBJECTLIST::iterator iterlist = QubelistMap[GAMEOBJECTID::CUBE_YELLOW].begin();
	OBJECTLIST::iterator iterlist_end = QubelistMap[GAMEOBJECTID::CUBE_YELLOW].end();

	for(; iterlist != iterlist_end; ++iterlist)
	{
		Engine::STANDARD_VERTEX3 StdVtx3;
		CYellowCube* pCube = static_cast<CYellowCube*>((*iterlist));

		pCube->GetChildMinMax(&StdVtx3);

		for(int i = 0; i < 3; ++i)
		{
			Engine::RAY3 Ray3 = OriginRay3;

			D3DXMATRIX matWorld = (*(pCube->GetCubeArray() + i))->m_matWorld;
			D3DXMATRIX matInvWorld;
			D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

			D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
			D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

			D3DXVECTOR3 vPos;
			if(IsRayIntersectedBox(StdVtx3, Ray3, &vPos, NULL))
			{				
				pCube->DeactiveCube();

				D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);
				m_pTextureParticlManager->AddParticle(vPos);

				return;
			}
		}
	}
}

void CCollisionManager::GetRay(Engine::RAY3* pRay3)
{
	D3DXVECTOR2 vMousePos(float(WINCX >> 1), float(WINCY >> 1));
	
	D3DXMATRIX matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3 vSrc;
	vSrc.x = (  vMousePos.x / (WINCX >> 1) - 1.f) / matProj._11;
	vSrc.y = ( -vMousePos.y / (WINCY >> 1) + 1.f) / matProj._22;
	vSrc.z = 1.f;

	pRay3->pos = D3DXVECTOR3(0.f, 0.f, 0.f);
	pRay3->dir = vSrc - pRay3->pos;
	D3DXVec3Normalize(&pRay3->dir ,&pRay3->dir);

	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&pRay3->pos, &pRay3->pos, &matView);
	D3DXVec3TransformNormal(&pRay3->dir, &pRay3->dir, &matView);
}

bool CCollisionManager::IsIntersectedOBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld)
{
	OBB Obb[2];
	ZeroMemory(Obb, sizeof(OBB) * 2);

	InitPoint(&Obb[0], pDestMin, pDestMax);
	InitPoint(&Obb[1], pSourMin, pSourMax);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&Obb[0].vPoint[i], &Obb[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&Obb[1].vPoint[i], &Obb[1].vPoint[i], pSourWorld);
	}
	D3DXVec3TransformCoord(&Obb[0].vCenter, &Obb[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&Obb[1].vCenter, &Obb[1].vCenter, pSourWorld);

	InitAxis(&Obb[0]);
	InitAxis(&Obb[1]);

	float		fDistance[3];

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&Obb[0].vProjAxis[0], &Obb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&Obb[0].vProjAxis[1], &Obb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&Obb[0].vProjAxis[2], &Obb[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&Obb[1].vProjAxis[0], &Obb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&Obb[1].vProjAxis[1], &Obb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&Obb[1].vProjAxis[2], &Obb[i].vAxis[j]));

			D3DXVECTOR3		vTemp = Obb[0].vCenter - Obb[1].vCenter;
			fDistance[2] = fabs(D3DXVec3Dot(&vTemp, &Obb[i].vAxis[j]));

			if(fDistance[2] > fDistance[0] + fDistance[1])
				return false;
		}
	}
	return true;
}

void CCollisionManager::InitPoint(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	pOBB->vPoint[0] = D3DXVECTOR3(pMin->x, pMax->y, pMin->z);
	pOBB->vPoint[1] = D3DXVECTOR3(pMax->x, pMax->y, pMin->z);
	pOBB->vPoint[2] = D3DXVECTOR3(pMax->x, pMin->y, pMin->z);
	pOBB->vPoint[3] = D3DXVECTOR3(pMin->x, pMin->y, pMin->z);

	pOBB->vPoint[4] = D3DXVECTOR3(pMin->x, pMax->y, pMax->z);
	pOBB->vPoint[5] = D3DXVECTOR3(pMax->x, pMax->y, pMax->z);
	pOBB->vPoint[6] = D3DXVECTOR3(pMax->x, pMin->y, pMax->z);
	pOBB->vPoint[7] = D3DXVECTOR3(pMin->x, pMin->y, pMax->z);

	for(int i = 0; i < 8; ++i)
		pOBB->vCenter += pOBB->vPoint[i];

	pOBB->vCenter *= 0.125f;
}

void CCollisionManager::InitAxis(OBB* pOBB)
{
	/*
	  4	  5	
	0---1/
    | 7 | 6
	3---2/
	*/
	//x
	pOBB->vProjAxis[0] = (pOBB->vPoint[1] + pOBB->vPoint[2] + pOBB->vPoint[5] + pOBB->vPoint[6]) * 0.25f
		- pOBB->vCenter;
	pOBB->vAxis[0] = pOBB->vPoint[2] - pOBB->vPoint[3];

	//y
	pOBB->vProjAxis[1] = (pOBB->vPoint[0] + pOBB->vPoint[1] + pOBB->vPoint[4] + pOBB->vPoint[5]) * 0.25f
		- pOBB->vCenter;
	pOBB->vAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];

	//z
	pOBB->vProjAxis[2] = (pOBB->vPoint[4] + pOBB->vPoint[5] + pOBB->vPoint[6] + pOBB->vPoint[7]) * 0.25f
		- pOBB->vCenter;
	pOBB->vAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for(int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pOBB->vAxis[i], &pOBB->vAxis[i]);
}

bool CCollisionManager::CollisionWithBlueCube(const Engine::STANDARD_VERTEX3& StdVtx3, const D3DXMATRIX& matWorld, D3DXVECTOR3* pDir)
{
	// 충돌 처리를 해서 충돌이 되었으면 큐브가 향하는 방향 값을 받아온다. 

	Engine::STANDARD_VERTEX3 JohnStdVtx3 = StdVtx3;
	D3DXMATRIX	matJohnWorld = matWorld;

	OBJECTLISTMAP& QubelistMap = m_QubelistMap;
	
	OBJECTLIST::iterator iterlist = QubelistMap[GAMEOBJECTID::CUBE_BLUE].begin();
	OBJECTLIST::iterator iterlist_end = QubelistMap[GAMEOBJECTID::CUBE_BLUE].end();

	for(; iterlist != iterlist_end; ++iterlist)
	{

		float fheight = matJohnWorld.m[3][1];

		float fheightCube =  ((CCube*)(*iterlist))->GetCubeData()->m_vPos.y;
		
		if(((CCube*)(*iterlist))->GetCubeData()->m_vPos.y + 60.f > matJohnWorld.m[3][1])
		{
			if((*iterlist)->Get_ViewZ() < 60.f)
			{
				if(((CBlueCube*)(*iterlist))->m_IsCharged)
				{
					Engine::STANDARD_VERTEX3 CubeStdVtx3;
					((CCube*)(*iterlist))->GetMinMax(&CubeStdVtx3);
					D3DXMATRIX matCubeWorld = ((CCube*)(*iterlist))->GetCubeData()->m_matWorld;

					if(IsIntersectedOBB(&JohnStdVtx3._min, &JohnStdVtx3._max, &matJohnWorld, &CubeStdVtx3._min, &CubeStdVtx3._max, &matCubeWorld))
					{
						((CBlueCube*)(*iterlist))->Discharge();
						*pDir = ((CCube*)(*iterlist))->GetCubeData()->m_vDir;
						return true;
					}
				}
			}
		}

		
	}
	return false;
}

const Engine::RAY3& CCollisionManager::GetRayState(void)
{
	return m_Ray3;
}


void CCollisionManager::Start220(void)
{
	OBJECTLISTMAP& ObjectlistMap = m_QubelistMap;

	Engine::CGameObject* pGameObject = NULL;

	for(int i = GAMEOBJECTID::CUBE_WHITE; i != GAMEOBJECTID::CUBE_END; ++i)
	{
		if(ObjectlistMap.count(i) == NULL)
			continue;

		OBJECTLIST::iterator iterlist = ObjectlistMap[i].begin();
		OBJECTLIST::iterator iterlist_end = ObjectlistMap[i].end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			if(((CCube*)(*iterlist))->GetCubeData()->m_vPos.z < 250.f)
			{
				((CCube*)(*iterlist))->RunUpdate();
			}
		}
	}
}

void CCollisionManager::Delete220(void)
{
	OBJECTLISTMAP& ObjectlistMap = m_QubelistMap;

	Engine::CGameObject* pGameObject = NULL;

	for(int i = GAMEOBJECTID::CUBE_WHITE; i != GAMEOBJECTID::CUBE_END; ++i)
	{
		if(ObjectlistMap.count(i) == NULL)
			continue;

		OBJECTLIST::iterator iterlist = ObjectlistMap[i].begin();
		OBJECTLIST::iterator iterlist_end = ObjectlistMap[i].end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			if(((CCube*)(*iterlist))->GetCubeData()->m_vPos.z < 200.f)
			{
				((CCube*)(*iterlist))->StopUpdate();
			}
		}
	}
}