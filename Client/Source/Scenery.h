/*!
 * \file Scenery.h
 * \date 2016/04/16 12:28
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
#ifndef Scenery_h__
#define Scenery_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CScenery : public Engine::CGameObject
{
public:
	enum COMPONENTID { TRANSFORM
					 , TEXBUFFER_RECTANGLE
					 , TEXTURE_RECTANGLE
					 , COMPONENTID_END };

private:
	explicit CScenery(LPDIRECT3DDEVICE9 pDevice);

	/// TODO: 복사 생성 방지
	explicit CScenery(const CScenery&);
	CScenery& operator = (const CScenery&);

public:
	virtual ~CScenery(void);

public:
	static CScenery* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	HRESULT InitScenery(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pState;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
};



#endif // Scenery_h__