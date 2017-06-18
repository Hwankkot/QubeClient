/*!
 * \file Cube.h
 * \date 2016/04/17 15:19
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

#ifndef Cube_h__
#define Cube_h__

#include "GameObject.h"
#include "Engine_Struct.h"

namespace Engine
{
	class CInput;
	class CLayer;
	class CRenderer;
	class CResourceMgr;
	class CStaticMesh;
	class CTransform;
	class CVIBuffer;
};


class CCube : public Engine::CGameObject
{
public:
	enum PLANE { PLANE_TOP, PLANE_BOT, PLANE_LEFT, PLANE_RIGHT, PLANE_FRONT, PLANE_BACK, PLANE_END };
	
public:
	static const float CUBESCALE;
	static const float CUBERADIUS;
	
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CCube(const CCube&);
	CCube& operator = (const CCube&);
  
public:
	virtual ~CCube(void);

public:
	virtual void Update(void) PURE;
	virtual void Render(void) PURE;

// 오브젝트 정보 관련
public:
	virtual const Engine::CTransform* const GetCubeData(void) const;	
	// TODO: 어차피 큐브들을 타입 별로 나누어서 저장해 관리하기 때문에
	// 타입 반환 함수가 필요 없음. 일단 놔두었다가 Collision이 완성 된 후
	// 필요 없게 되면 지울 것.
	virtual DWORD GetCubeType(void);									// 큐브 타입 반환

public:
	virtual LPD3DXMESH GetMesh(void);
	virtual void GetMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3);

public:
	virtual void ActiveCube(void)	{}
	virtual void DeactiveCube(void) {}

public:
	void RunUpdate(void);
	void StopUpdate(void);
	
// 컴포넌트 및 매니저 포인터
protected:
	Engine::CInput*			m_pInput;
	Engine::CLayer*			m_pLayer;
	Engine::CRenderer*		m_pRenderer;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTransform*		m_pState;

	// Static Mesh의 경우 Yellow 큐브 같은 것은 따로 생성해서 사용하기 때문에 
	// 변수를 상속받아서 사용할 수 가 없다.
	Engine::CStaticMesh*	m_pStaticMesh;
	Engine::CVIBuffer*		m_pSphere;

protected:
	bool m_IsUpdate;
	bool m_IsIntersectedtoSelect;
	bool m_IsSelected;
	DWORD m_dwCubeType;
};

#endif // Cube_h__
