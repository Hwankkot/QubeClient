#include "stdafx.h"
#include "MainApp.h"

#include "CollisionManager.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "JohnStateSubject.h"
#include "SelectScene.h"
#include "SoundMgr.h"
#include "Value.h"

//-----------------------------------------------------
// 공통 전역변수
//-----------------------------------------------------


// 윈도우 창 크기 관련
const WORD WINCX = 1980;			// 디바이스 출력 창 크기
const WORD WINCY = 1080;				

// 고정 수치
const float FLOATMAX = 3.402823466e+38f;		// float Max값
const float JOHNHEIGHT = 8.f;

const D3DXVECTOR3	g_vLook(0.f, 0.f, 1000.f);		// 기본 Look벡터
const D3DXVECTOR3	g_vUp(0.f, 1.f, 0.f);
const D3DXVECTOR3	g_vDown(0.f, -1.f, 0.f);
const D3DXVECTOR3	g_vGravity(0.f, -9.8f, 0.f);

//-----------------------------------------------------

CMainApp::CMainApp(void)
: m_pDevice(NULL)
, m_pInput(NULL)
, m_pManagement(NULL)
, m_pTimeMgr(NULL)
{

}

CMainApp::~CMainApp(void)
{
	Release();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp* pMainApp = new CMainApp;

	if(FAILED(pMainApp->InitMainApp()))
		Engine::Safe_Delete(pMainApp);

	return pMainApp;
}

HRESULT CMainApp::InitMainApp(void)
{
	HRESULT hr = NULL;

	hr = Engine::Get_GraphicDev()->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);
	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
	m_pInput = Engine::Get_Input();
	m_pManagement = Engine::Get_Management();
	m_pRenderer = Engine::Get_Renderer();
	m_pTimeMgr = Engine::Get_TimeMgr();
	
	CSoundMgr::GetInstance()->LoadSoundFile();

	Engine::Get_NavMgr()->InitNavMeshManager(m_pDevice, 100);
	
	// 첫 이미지 버퍼
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
		, Engine::BUFFER_RCTEX
		, RESOURCEID::TEXBUFFER_RECTANGLE);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Failed to create Buffer_RcTex");

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, Engine::TEXTURE_NORMAL
		, RESOURCEID::RECTTEXTURE_LOGO
		, L"../../Data/Resources/Texture/Logo.tga"
		, 1);		
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Failed to create Logo.tga");

	// 입력장치 초기화
	m_pInput->InitInputDevice(g_hInst, g_hWnd);
	
	hr = m_pManagement->InitManagement(m_pDevice);
	hr = m_pManagement->ChangeScene(CSelectScene(CSelectScene::SCENE_LOGO));
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Failed to Change Scene ");

	hr = Engine::Get_FontMgr()->AddFont(m_pDevice, L"Cambria", 16, 12, FW_LIGHT);
	FAILED_CHECK(hr);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

void CMainApp::Update(void)
{
	m_pInput->SetInputStateData();	// 장치(마우스, 키보드) 입력을 갱신한다.

	m_pManagement->Update();		// 현재 Scene 업데이트
}

void CMainApp::Render(void)
{
	float fTime = m_pTimeMgr->GetTime();
	m_pRenderer->Render(fTime);
}

void CMainApp::Release(void)
{
	// System
	Engine::Get_FontMgr()->DestroyInstance();			// 폰트 디바이스
	Engine::Get_Input()->DestroyInstance();				// 입력장치
	Engine::Get_GraphicDev()->DestroyInstance();		// 컴 객체
	Engine::Get_TimeMgr()->DestroyInstance();			// 타임


	// Utility
	Engine::Get_Management()->DestroyInstance();
	Engine::Get_Renderer()->DestroyInstance();
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_NavMgr()->DestroyInstance();

	// Resources
	Engine::Get_ResourceMgr()->DestroyInstance();

	// Client
	CCollisionManager::GetInstance()->DestroyInstance();
	CJohnStateSubject::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();

}