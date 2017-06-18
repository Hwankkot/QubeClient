/*!
 * \file GrayCube.h
 * \date 2016/05/02 23:35
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
#ifndef GrayCube_h__
#define GrayCube_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CGrayCube: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
		  			 , COMPONENTID_END };
protected:
	explicit CGrayCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CGrayCube(const CGrayCube&);
	CGrayCube& operator = (const CGrayCube&);

public:
	virtual ~CGrayCube(void);

public:
	static CGrayCube* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);

private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);


	void Release(void);
};

#endif // GrayCube_h__