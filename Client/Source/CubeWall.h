/*!
 * \file CubeWall.h
 * \date 2016/05/02 23:34
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
#ifndef CubeWall_h__
#define CubeWall_h__

#include "Cube.h"
#include "Engine_Struct.h"

class CCubeWall: public CCube
{
public:
	enum COMPONENTID { TRANSFORM
					 , COLBUFFER_SPHERE
					 , MESH_STATIC
					 , COMPONENTID_END };

protected:
	explicit CCubeWall(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CCubeWall(const CCubeWall&);
	CCubeWall& operator = (const CCubeWall&);

public:
	virtual ~CCubeWall(void);

public:
	static CCubeWall* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CUBEDATA* pCubeData = NULL);
private:
	HRESULT InitCube(const Engine::CUBEDATA* pCubeData);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);
};

#endif // CubeWall_h__