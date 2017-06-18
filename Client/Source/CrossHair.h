/*!
 * \file CrossHair.h
 * \date 2016/05/10 17:31
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

#ifndef CrossHair_h__
#define CrossHair_h__


#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CTexture;
	class CVIBuffer;
};

class CCrossHair : public Engine::CGameObject
{
public:
	enum COMPONENTID { TEXBUFFER_CROSSHAIR, RECTTEXTURE_CROSSHAIR, COMPONENTID_END };

private:
	explicit CCrossHair(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCrossHair(void);

public:
	static CCrossHair* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT InitCrossHair(void);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);

// 컴포넌트 및 매니저 포인터
private:
	Engine::CResourceMgr*	m_pResourceMgr;


// 그리기 관련
private:
	Engine::CTexture*		m_pCrossHairTex;
	Engine::CVIBuffer*		m_pCrossHairBuffer;

	float m_fSizeX;
	float m_fSizeY;

	D3DXMATRIX m_matOrthoProj;
	D3DXMATRIX m_matOrthoView;
	D3DXVECTOR2 m_vCrossHair;
};


#endif // CrossHair_h__