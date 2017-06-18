#include "stdafx.h"
#include "Logo.h"

#include "CollisionManager.h"
#include "Enum.h"
#include "Engine_Function.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "Loading.h"
#include "Scenery.h"
#include "SelectScene.h"
#include "Struct.h"
#include "VIBuffer.h"

/// Cube
#include "BlueCube.h"
#include "RedCube.h"
#include "YellowCube.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pRenderer(NULL)
, m_pLoading(NULL)
{

}

CLogo::~CLogo(void)
{
	Release();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pScene = new CLogo(pDevice);

	if(FAILED(pScene->InitLogo()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CLogo::InitLogo(void)
{
	/// 빛을 끄고 Stage에서 빛을 다시 부른다.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pRenderer = Engine::Get_Renderer();

	/// 메쉬를 로딩할 스레드를 만든다.
	m_pLoading = CLoading::Create();
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	HRESULT hr = NULL;

	// Scenery 추가
	AddEnvironmentLayer();

	return S_OK;
}

HRESULT CLogo::AddEnvironmentLayer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CScenery::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Failed to create Scenery");

	Engine::Get_Renderer()->AddRenderObject(Engine::CRenderer::RENDER_UI, pGameObject);
	pLayer->AddObject(GAMEOBJECTID::ENV_SCENERY, pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));

	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();

	if(m_pLoading->GetComplete() == true)
	{
		// TODO: Change to KeyMgr
		if(GetAsyncKeyState(VK_RETURN))
		{
			Engine::Get_Renderer()->ResetRenderlist(Engine::CRenderer::RENDER_END);
			Engine::CLayer* pLayer = m_pLoading->GetCubeLayer();
			HRESULT hr = Engine::Get_Management()->ChangeScene(CSelectScene(CSelectScene::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, , L"Failed to change Scene");

			CCollisionManager::GetInstance()->AddQubeObjectMap(pLayer->GetMapObjectlist());
			Engine::Get_Management()->AddLayer(CStage::LAYER_QUBE, pLayer, Engine::CRenderer::RENDER_CUBE);

			/// Scene이 바뀐 이후에 Render를 바로 실행하게 되므로 Update를 강제적으로 돌려준다.
			/// Logo에 있던 m_pManagement 포인터는 소실되므로 직접 함수를 호출해서 Update한다.
			Engine::Get_Management()->Update();
			return ;
		}
	}
	m_pRenderer->SetMessage(m_pLoading->GetLoadingMessage());
}

void CLogo::Release(void)
{
	Engine::Safe_Delete(m_pLoading);
}
