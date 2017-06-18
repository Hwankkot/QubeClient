/*!
 * \file GreenCube.h
 * \date 2016/05/02 23:37
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

#ifndef GreenCube_h__
#define GreenCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CGreenCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };
protected:
	explicit CGreenCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CGreenCube(const CGreenCube&);
	CGreenCube& operator = (const CGreenCube&);

public:
	virtual ~CGreenCube(void);

public:
	static CGreenCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);
};

#endif // GreenCube_h__