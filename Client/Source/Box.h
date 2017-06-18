#ifndef Box_h__
#define Box_h__

#include "Camera.h"
#include "Engine_Struct.h"

namespace Engine
{
	class CDynamicMesh;
	class CInput;
	class CLayer;
	class CNavMgr;
	class CRenderer;
	class CResourceMgr;
	class CTexture;
	class CTimeMgr;
	class CTransform;
	class CVIBuffer;

	class CStaticMesh;
};

class CCollisionManager;
class CBox : public Engine::CGameObject
{
public:
	enum COMPONENTID { TRANSFORM
					 , TEXBUFFER_CROSSHAIR
					 , RECTTEXTURE_CROSSHAIR
					 , MESH_DYNAMIC
					 , MESH_STATIC
					 , COMPONENTID_END
	};

private:
	explicit CBox(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBox(void);

public:
	static CBox* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vPos);

private:
	HRESULT InitBox(const D3DXVECTOR3& vPos);


private:
	NxActor* CreateBox(void);

	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);

// 업데이트 관련
private:
	void UpdateUnitMove(void);
	void ReviseUnitPosHeight(D3DXVECTOR3* pPos);
	void UpdateTransform(void);

// 컴포넌트 및 매니저 포인터
private:
	Engine::CDynamicMesh*	m_pDynamicMesh;
	Engine::CTransform*		m_pState;
	Engine::CInput*			m_pInput;
	Engine::CLayer*			m_pLayer;
	Engine::CNavMgr*		m_pNavMgr;
	Engine::CRenderer*		m_pRenderer;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;



// 그리기 관련
private:
	Engine::CTexture*		m_pCrossHairTex;
	Engine::CVIBuffer*		m_pCrossHairBuffer;

// 피직스 관련
	NxVec3 ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);
	


// 정리 중


private:
	bool		m_IsPosChanged;
	float		m_fVelocity;
	DWORD		m_dwIndex;
	UINT		m_iAniIdx;
private:
	TCHAR		m_szMessage[128];


//-------------------------------------
// CrossHair 관련

	/*D3DXMATRIX m_matOrthoProj;
	D3DXMATRIX m_matOrthoView;*/
	D3DXMATRIX m_matOrthoWorld;
	float m_fSizeX;
	float m_fSizeY;
	D3DXVECTOR2 m_vCrossHair;
	D3DXMATRIX m_matOrthoProj;
	D3DXMATRIX m_matOrthoView;
//-------------------------------------

// ------------------------------------
// Test Variable

	Engine::CVIBuffer*		m_pSphere;
	Engine::CVIBuffer*		m_pBoundingBox;

	Engine::STANDARD_VERTEX3 m_StdVtx3;
	float		m_fDistance;
	CCollisionManager* m_pCollisionManager;
// ------------------------------------





// PhysX

	Engine::CStaticMesh* m_pStaticMesh;
	NxScene*	m_pNxScene;
	NxActor*	m_pBoxActor;

	NxVec3		m_ForceVec;

	D3DXMATRIX	m_matWorld;
	
	
};

#endif // Box_h__