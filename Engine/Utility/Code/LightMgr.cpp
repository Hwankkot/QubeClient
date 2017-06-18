#include "LightMgr.h"

#include "Engine_Functor.h"
#include "Light.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Release();
}

HRESULT Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice 
									, const D3DLIGHT9* pLightState 
									, const DWORD& dwLightIdx)
{
	if(pDevice == NULL || pLightState == NULL)
		return E_FAIL;

	CLight*		pLight = CLight::Create(pDevice, pLightState, dwLightIdx);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_Lightlist.push_back(pLight);
	return S_OK;
}

void Engine::CLightMgr::Release(void)
{
	for_each(m_Lightlist.begin(), m_Lightlist.end(), CDeleteObj());
	m_Lightlist.clear();
}

