/*!
 * \file TextureParticleManager.h
 * \date 2016/05/11 15:31
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
#ifndef TextureParticleManager_h__
#define TextureParticleManager_h__

#include "Engine_Macro.h"

namespace Engine
{
	class CRenderer;
}

class CPickingParticle;
class CTextureParticleManager
{
	DECLARE_SINGLETON(CTextureParticleManager)
	
private:
	CTextureParticleManager(void);
	~CTextureParticleManager(void);

public:
	void AddParticle(const D3DXVECTOR3& vPos);
	void Update(void);

private:
	void Release(void);
	

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	Engine::CRenderer*	m_pRenderer;
private:
	size_t	m_iSize;

	typedef std::vector<CPickingParticle*>	 PARTICLELIST;
	PARTICLELIST		m_Particlelist;
};

#endif // TextureParticleManager_h__