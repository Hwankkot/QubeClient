#include "stdafx.h"
#include "TextureParticleManager.h"

#include "Engine_Function.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "PickingParticle.h"

IMPLEMENT_SINGLETON(CTextureParticleManager)

CTextureParticleManager::CTextureParticleManager(void)
: m_pRenderer(Engine::Get_Renderer())
, m_pDevice(Engine::Get_GraphicDev()->GetDevice())
, m_iSize(0)
{
	m_Particlelist.reserve(200);
}

CTextureParticleManager::~CTextureParticleManager(void)
{
	Release();
}

void CTextureParticleManager::AddParticle(const D3DXVECTOR3& vPos)
{
	CPickingParticle* pParticle =CPickingParticle::Create(m_pDevice, vPos);

	m_Particlelist.push_back(pParticle);
	++m_iSize;
	m_pRenderer->AddRenderObject(Engine::CRenderer::RENDER_UI, pParticle);
}

void CTextureParticleManager::Update(void)
{
	for(size_t i = 0; i < m_iSize; ++i)
	{
		m_Particlelist[i]->Update();
		if(m_Particlelist[i]->m_IsEnd)
		{
			m_pRenderer->DeleteRenderObject(Engine::CRenderer::RENDER_UI, m_Particlelist[i]);
			CPickingParticle* pParticleSrc = m_Particlelist[i];
			m_Particlelist[i] = m_Particlelist[m_iSize - 1];
			Engine::Safe_Delete(pParticleSrc);
			m_Particlelist.pop_back();
			--m_iSize;
		}
	}
}

void CTextureParticleManager::Release(void)
{
	for(size_t i = 0; i < m_iSize; ++i)
	{
		if(m_Particlelist[i] != NULL)
		{
			//m_pRenderer->DeleteRenderObject(Engine::CRenderer::RENDER_UI, m_Particlelist[i]);
			Engine::Safe_Delete( m_Particlelist[i]);
		}
		
	}
}
