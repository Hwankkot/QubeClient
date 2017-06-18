/*!
 * \file SkyDorm.h
 * \date 2016/05/06 23:34
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
#ifndef SkyDorm_h__
#define SkyDorm_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
};

class CJohnStateObserver;
class CJohnStateSubject;
class CSkyDorm : public Engine::CGameObject
{
public:
	enum COMPONENTID { TRANSFORM
					 , MESH_STATIC
					 , COMPONENTID_END
	};


private:
	explicit CSkyDorm(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkyDorm(void);

public:
	static CSkyDorm* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT InitSkyDorm(void);
	HRESULT AddComponent(void);
	virtual void Update(void);
	virtual void Render(void);
	void Release(void);


private:
	Engine::CTransform*		m_pState;
	Engine::CStaticMesh*	m_pStaticMesh;
	CJohnStateObserver*		m_pJohnStateObserver;
	CJohnStateSubject*		m_pJohnStateSubject;
};


#endif // SkyDorm_h__