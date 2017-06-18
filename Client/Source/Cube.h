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

// ������Ʈ ���� ����
public:
	virtual const Engine::CTransform* const GetCubeData(void) const;	
	// TODO: ������ ť����� Ÿ�� ���� ����� ������ �����ϱ� ������
	// Ÿ�� ��ȯ �Լ��� �ʿ� ����. �ϴ� ���ξ��ٰ� Collision�� �ϼ� �� ��
	// �ʿ� ���� �Ǹ� ���� ��.
	virtual DWORD GetCubeType(void);									// ť�� Ÿ�� ��ȯ

public:
	virtual LPD3DXMESH GetMesh(void);
	virtual void GetMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3);

public:
	virtual void ActiveCube(void)	{}
	virtual void DeactiveCube(void) {}

public:
	void RunUpdate(void);
	void StopUpdate(void);
	
// ������Ʈ �� �Ŵ��� ������
protected:
	Engine::CInput*			m_pInput;
	Engine::CLayer*			m_pLayer;
	Engine::CRenderer*		m_pRenderer;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTransform*		m_pState;

	// Static Mesh�� ��� Yellow ť�� ���� ���� ���� �����ؼ� ����ϱ� ������ 
	// ������ ��ӹ޾Ƽ� ����� �� �� ����.
	Engine::CStaticMesh*	m_pStaticMesh;
	Engine::CVIBuffer*		m_pSphere;

protected:
	bool m_IsUpdate;
	bool m_IsIntersectedtoSelect;
	bool m_IsSelected;
	DWORD m_dwCubeType;
};

#endif // Cube_h__
