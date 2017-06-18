/*!
 * \file PurpleCube.h
 * \date 2016/05/02 23:31
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

#ifndef PurpleCube_h__
#define PurpleCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CPurpleCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };
protected:
	explicit CPurpleCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CPurpleCube(const CPurpleCube&);
	CPurpleCube& operator = (const CPurpleCube&);

public:
	virtual ~CPurpleCube(void);

public:
	static CPurpleCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);
};

#endif // PurpleCube_h__