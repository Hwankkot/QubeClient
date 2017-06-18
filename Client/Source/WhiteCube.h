/*!
 * \file WhiteCube.h
 * \date 2016/04/27 20:11
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
#ifndef WhiteCube_h__
#define WhiteCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CJohnStateObserver;
class CWhiteCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };
protected:
	explicit CWhiteCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CWhiteCube(const CWhiteCube&);
	CWhiteCube& operator = (const CWhiteCube&);

public:
	virtual ~CWhiteCube(void);

public:
	static CWhiteCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);

private:
	CJohnStateObserver* m_pJohnStateObserver;
	D3DXVECTOR3	m_vOrigin;
	D3DXVECTOR3	m_vLeft;
	D3DXVECTOR3	m_vRight;
	float	m_fDistance;
	Engine::STANDARD_VERTEX3	m_StdVtx3;
	int m_iMulti;
	
};

#endif // WhiteCube_h__
