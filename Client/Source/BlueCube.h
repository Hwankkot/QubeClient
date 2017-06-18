/*!
 * \file BlueCube.h
 * \date 2016/05/02 23:38
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
#ifndef BlueCube_h__
#define BlueCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CBlueCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };


protected:
	explicit CBlueCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CBlueCube(const CBlueCube&);
	CBlueCube& operator = (const CBlueCube&);

public:
	virtual ~CBlueCube(void);

public:
	static CBlueCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);

public:
	void ActiveCube(void);
	void DeactiveCube(void);
	void Discharge(void);
	void MovingCube(void);

private:
	bool m_IsActived;
	bool m_IsDeactive;

public:
	bool m_IsCharged;
	bool m_IsDisCharged;

	D3DXVECTOR3 m_vOriginPos;
	D3DXVECTOR3 m_vMovedPos;
};

#endif // BlueCube_h__