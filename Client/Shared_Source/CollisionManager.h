/*!
 * \file CollisionManager.h
 * \date 2016/05/05 14:44
 *
 * \author Architect
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef CollisionManager_h__
#define CollisionManager_h__

#include "Engine_Macro.h"
#include "Engine_Struct.h"

namespace Engine
{
	class CGameObject;
}

class CTextureParticleManager;
class CCollisionManager
{
	DECLARE_SINGLETON(CCollisionManager)

public:
	typedef struct tagOBB
	{
		D3DXVECTOR3		vPoint[8];
		D3DXVECTOR3		vCenter;
		D3DXVECTOR3		vProjAxis[3];
		D3DXVECTOR3		vAxis[3];
	}OBB;

private:
	CCollisionManager(void);
	~CCollisionManager(void);

// 충돌 처리 할 리스트 관련

public:
	void AddQubeObjectMap(std::map<DWORD, std::vector<Engine::CGameObject*>>* pQubeObjectlistMap);
	void RemoveQubeObjectMap(void);

public:
	void Update(void);
	void UpdateCullingQube(void);
	
public:
	void CollisionActiveCube(void);
	void CollisionDeactiveCube(void);

	

public:
	bool CollisionWithBlueCube(const Engine::STANDARD_VERTEX3& StdVtx3, const D3DXMATRIX& matWorld, D3DXVECTOR3* pDir);

private:
	void GetRay(Engine::RAY3* pRay3);

private:
	void InitPoint(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);
	void InitAxis(OBB* pOBB);


// 충돌 처리 관련
public:
	bool IsRayIntersectedSphere(const Engine::RAY3& Ray3, const D3DXVECTOR3& vSphereCenter, const float& fRadius, D3DXVECTOR3* pPos); // 구충돌
	bool IsRayIntersectedBox(const Engine::STANDARD_VERTEX3& StdVtx3, const Engine::RAY3& Ray3, D3DXVECTOR3* pPos, WORD* pEdge);	  // 박스 충돌
	bool IsRayIntersectedMeshtoReviseHeight(D3DXVECTOR3* pPos); // 메시 충돌 (높이 값 얻기)
	bool IsIntersectedOBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld
		, const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld);


public:
	const Engine::RAY3& GetRayState(void);

	void Start220(void);
	void Delete220(void);

// 파티클 관련
CTextureParticleManager* m_pTextureParticlManager;

private:
	typedef std::vector<Engine::CGameObject*>	OBJECTLIST;
	typedef std::map<DWORD, OBJECTLIST>			OBJECTLISTMAP;
	OBJECTLISTMAP*		m_pQubelistMap;
	OBJECTLISTMAP	m_QubelistMap;

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	Engine::RAY3 m_Ray3;
	bool	m_IsCreatedParticle;
};

#endif // CollisionManager_h__