#include "Light.h"

#include "Engine_Function.h"

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	ZeroMemory(&m_LightState, sizeof(D3DLIGHT9));
}

Engine::CLight::~CLight(void)
{

}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightState, const DWORD& dwLightIdx)
{
	CLight*		pLight = new CLight(pDevice);
	if(FAILED(pLight->InitLight(pLightState, dwLightIdx)))
		Safe_Delete(pLight);

	return pLight;
}

HRESULT Engine::CLight::InitLight(const D3DLIGHT9* pLightState, const DWORD& dwLightIdx)
{
	memcpy(&m_LightState, pLightState, sizeof(D3DLIGHT9));

	m_pDevice->SetLight(dwLightIdx, &m_LightState);
	m_pDevice->LightEnable(dwLightIdx, TRUE);
	return S_OK;
}

