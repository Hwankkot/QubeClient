/*!
 * \file RedCube.h
 * \date 2016/05/02 23:43
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
#ifndef RedCube_h__
#define RedCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CRedCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };
protected:
	explicit CRedCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CRedCube(const CRedCube&);
	CRedCube& operator = (const CRedCube&);

public:
	virtual ~CRedCube(void);

public:
	static CRedCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void ActiveCube(void);
	virtual void DeactiveCube(void);
	void MovingCube(void);
	void Release(void);

public:
	int	m_iMovedCount;

private:
	bool m_IsActived;
	bool m_IsDeactive;

	D3DXVECTOR3 m_vOriginPos;
	D3DXVECTOR3 m_vMovedPos;
	D3DXVECTOR3 m_vMaxPos;

	float m_fDistance;
};

#endif // RedCube_h__