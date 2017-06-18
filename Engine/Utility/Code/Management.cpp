#include "Management.h"

#include "Engine_Function.h"
#include "Export_Utility.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pScene(NULL)
, m_pRenderer(NULL)
, m_pDevice(NULL)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pRenderer = Get_Renderer();

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Delete(m_pScene);
}

void Engine::CManagement::AddLayer(const DWORD& dwContainerKey, Engine::CLayer* pLayer, const DWORD& dwRenderID)
{
	NULL_CHECK_RETURN_MSG(pLayer, , L"The container is empty.");
	NULL_CHECK_RETURN_MSG(m_pScene, , L"The scnene is not ready.");

	m_pScene->AddLayer(dwContainerKey, pLayer, dwRenderID);
}