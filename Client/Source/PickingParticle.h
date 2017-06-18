/*!
 * \file PickingParticle.h
 * \date 2016/05/11 14:53
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

#ifndef PickingParticle_h__
#define PickingParticle_h__

#include "Engine_Struct.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CTimeMgr;
}

class CPickingParticle : public Engine::CGameObject
{
public:
	enum COMPONENTID { TRANSFORM, TEXBUFFER_PARTICLE, RECTTEXTURE_PARTICLE, COMPONENTID_END };
private:
	explicit CPickingParticle(LPDIRECT3DDEVICE9 pDevice);
public:
	~CPickingParticle(void);

public:
	static CPickingParticle* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vPos);

private:
	virtual HRESULT InitPickingPartcle(const D3DXVECTOR3& vPos);

private:
	HRESULT AddComponent(void);
	void SetTransform(void);

public:
	virtual void Update(void);
	virtual void Render(void);
private:
	void Release(void);

private:
	Engine::CVIBuffer*	m_pBuffer;
	Engine::CTexture*	m_pTexture;
	Engine::CTimeMgr*	m_pTimeMgr;


private:
	D3DXMATRIX m_matWorld;

	float m_fTexutreIndex;

public:
	bool m_IsEnd;
};

#endif // PickingParticle_h__