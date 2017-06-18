/*!
 * \file John.h
 * \date 2016/05/04 10:21
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

#ifndef John_h__
#define John_h__

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
	class CTimeMgr;
	class CTransform;
	class CVIBuffer;
};


class CJohnStateSubject;
class CCollisionManager;
class CJohn : public Engine::CCamera
{
public:
	enum COMPONENTID { TRANSFORM
					 , TEXBUFFER_CROSSHAIR
					 , RECTTEXTURE_CROSSHAIR
					 , MESH_DYNAMIC
					 , COMPONENTID_END
	};

private:
	explicit CJohn(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CJohn(void);

public:
	static CJohn* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt);

private:
	HRESULT InitJohn(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);


// ������Ʈ ����
private:
	void UpdateUnitMove(void);				// ������
	void UpdateUnitRotate(void);			// ī�޶� ȸ��
	void ReviseUnitPos(D3DXVECTOR3* pEye);	// ����	����
	void UpdateMeshAnimation(void);

private:
	void UpdateParabolaMovement(const float& fTime);


// ������Ʈ �� �Ŵ��� ������
private:
	Engine::CDynamicMesh*	m_pDynamicMesh;
	Engine::CTransform*		m_pState;
	Engine::CInput*			m_pInput;
	Engine::CLayer*			m_pLayer;
	Engine::CNavMgr*		m_pNavMgr;
	Engine::CRenderer*		m_pRenderer;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;



// ������ ����
	CJohnStateSubject*		m_pJohnStateSubject;


// �浹 ���� (John Body)
private:
	Engine::STANDARD_VERTEX3	m_StdVtx3;
	CCollisionManager* m_pCollisionManager;


// ������ ����
private:
	bool		m_IsJumped;
	bool		m_IsFlew;
	bool		m_IsPosChanged;
	bool		m_bPush;
	DWORD		m_dwIndex;	// NavMesh
	float		m_fSpeed;	// �⺻ �̵� �ӵ�
	float		m_fOriginHeight;
	float		m_fJumpTime;
	float		m_fJumpVelocity;
	float		m_fCacheJumpHeight;

// Parabola Movemnet update ����
private:
	D3DXVECTOR3 m_vOriginPos;
	D3DXVECTOR3 m_vParabolaDir;
	float		m_fVelocity;
	float		m_fReviseVelocity;
	float		m_fPtime;
	float		m_fAddPower;
	float		m_fCacheParabolaHeight;

// �ܼ� �ڵ�
	D3DXVECTOR3 m_vVelocity;
	


// �ִϸ��̼�
private:
	UINT		m_iAniIdx;
private:
	TCHAR		m_szMessage[128];


// ���� ����

private:
	bool m_IsStart;
	bool m_IsSHow;

};

#endif // John_h__