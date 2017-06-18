#include "stdafx.h"
#include "Stage.h"

#include "CollisionManager.h"
#include "CrossHair.h"
#include "Engine_Function.h"
#include "Enum.h"
#include "Export_System.h" /// for using font
#include "Export_Utility.h"
#include "John.h"
#include "Layer.h"
#include "Macro.h"
#include "Renderer.h"
#include "SoundMgr.h"
#include "SkyDorm.h"
#include "Value.h"

// Cube
#include "WhiteCube.h"
#include "BlueCube.h"
#include "CubeWall.h"


CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pRenderer(NULL)
{

}

CStage::~CStage(void)
{
	
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pScene = new CStage(pDevice);

	if(FAILED(pScene->InitScene()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CStage::InitScene(void)  
{
	m_pRenderer = Engine::Get_Renderer();
	
	FAILED_CHECK(AddUI());
	FAILED_CHECK(AddEnvironment());;
	FAILED_CHECK(AddLight());
	
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	CSoundMgr::GetInstance()->PlayBgm(L"Fly_Me_To_The_Moon.mp3");

	return S_OK;
}

HRESULT CStage::AddUI(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(m_pDevice);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CCrossHair::Create(Engine::Get_GraphicDev()->GetDevice());
	pLayer->AddObject(GAMEOBJECTID::UI_CROSSHAIR, pGameObject);
	m_pRenderer->AddRenderObject(Engine::CRenderer::RENDER_UI, pGameObject);

	pGameObject = CJohn::Create(m_pDevice, D3DXVECTOR3(0.f, JOHNHEIGHT + 2.f, 10.f), g_vLook);
	pLayer->AddObject(GAMEOBJECTID::UI_PLAYUNIT, pGameObject);
	m_pRenderer->AddRenderObject(Engine::CRenderer::RENDER_UI, pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	
	return S_OK;
}

HRESULT CStage::AddEnvironment(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(m_pDevice);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CSkyDorm::Create(m_pDevice);
	pLayer->AddObject(GAMEOBJECTID::UI_PLAYUNIT, pGameObject);

	m_pRenderer->AddRenderObject(Engine::CRenderer::RENDER_PRIORITY, pGameObject);
	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVINRONMENT, pLayer));;

	return S_OK;
}

HRESULT CStage::AddLight(void)
{
	D3DLIGHT9		LightState;
	ZeroMemory(&LightState, sizeof(D3DLIGHT9));

	LightState.Type = D3DLIGHT_DIRECTIONAL;
	LightState.Diffuse	= D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	LightState.Ambient	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightState.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightState.Direction = D3DXVECTOR3(0.f, -50.f, 100.f);

	FAILED_CHECK(Engine::Get_LightMgr()->AddLight(m_pDevice, &LightState, 0));
	
	return S_OK;
}

void CStage::Update(void)
{
	CCollisionManager::GetInstance()->Update();

	Engine::CScene::Update();
}

void CStage::AddLayer(const DWORD& dwContainerKey, Engine::CLayer* pLayer, const DWORD& dwRenderID)
{
	int iResult = m_mapLayer.count(dwContainerKey);
	EXIST_CHECK_RETURN_MSG(iResult, , L"The container is alreay exisit.");

	m_mapLayer.insert(MAPLAYER::value_type(dwContainerKey, pLayer));
 
	typedef std::vector<Engine::CGameObject*>	OBJECTLIST;
	typedef std::map<DWORD, OBJECTLIST>		OBJECTLISTMAP;
	OBJECTLISTMAP* pmapObjectlist;

	pmapObjectlist = pLayer->GetMapObjectlist();
	
	OBJECTLISTMAP::iterator mapiter = pmapObjectlist->begin();
	OBJECTLISTMAP::iterator mapiter_end = pmapObjectlist->end();

	for(; mapiter != mapiter_end; ++mapiter)
	{
		OBJECTLIST::iterator iterlist = mapiter->second.begin();
		OBJECTLIST::iterator iterlist_end = mapiter->second.end();

		for(; iterlist != iterlist_end; ++iterlist)
		{
			m_pRenderer->AddRenderObject(Engine::CRenderer::RENDERID(dwRenderID), (*iterlist));
		}
	}
}