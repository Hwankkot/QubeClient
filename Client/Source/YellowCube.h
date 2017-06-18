/*!
 * \file YellowCube.h
 * \date 2016/05/02 23:42
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
#ifndef YellowCube_h__
#define YellowCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CCollisionManager;
class CYellowCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORMPOS_LEFT
					 , TRNASFORMPOS_MID
					 , TRANSFORMPOS_RIGHT
					 , MESH_STATICPOS_LEFT
					 , MESH_STATICPOS_MID
					 , MESH_STATICPOS_RIGHT
					 , COMPONENTID_END };


	enum { POS_LEFT, POS_MID, POS_RIGHT, POS_END };
protected:
	explicit CYellowCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CYellowCube(const CYellowCube&);
	CYellowCube& operator = (const CYellowCube&);

public:
	virtual ~CYellowCube(void);

public:
	static CYellowCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);

public:
	virtual void GetMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3);
	void GetChildMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3);
	const Engine::CTransform* const GetCubeData(void) const;
	Engine::CTransform** GetCubeArray(void);
	LPD3DXMESH* GetMeshArray(void);

public:
	void ActiveCube(void);
	void DeactiveCube(void);

public:
	Engine::STANDARD_VERTEX3	m_RevisedStdVtx3;
	Engine::STANDARD_VERTEX3	m_StdVtx3;
	Engine::CTransform*			m_pStateArray[POS_END];
	Engine::CStaticMesh*		m_pStaticMesh[POS_END];
	D3DXVECTOR3					m_vOriginPos[POS_END];
	D3DXVECTOR3					m_vMaxPos[POS_END];
	D3DXVECTOR3					m_vMovedPos[POS_END];
	LPD3DXMESH					m_pArrStaticMesh[POS_END];

	float m_fDistance;

	bool m_IsSelected[POS_END];

private:
	bool m_IsActived;
	bool m_IsDeactive;

private:
	CCollisionManager* m_pCollisionMgr;



	
};

#endif // YellowCube_h__