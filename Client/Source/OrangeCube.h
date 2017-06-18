/*!
 * \file OrangeCube.h
 * \date 2016/05/02 23:45
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
#ifndef OrangeCube_h__
#define OrangeCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class COrangeCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };
protected:
	explicit COrangeCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit COrangeCube(const COrangeCube&);
	COrangeCube& operator = (const COrangeCube&);

public:
	virtual ~COrangeCube(void);

public:
	static COrangeCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);
};

#endif // OrangeCube_h__