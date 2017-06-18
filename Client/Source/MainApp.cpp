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
// ���� ��������
//-----------------------------------------------------


// ������ â ũ�� ����
const WORD WINCX = 1980;			// ����̽� ��� â ũ��
const WORD WINCY = 1080;				

// ���� ��ġ
const float FLOATMAX = 3.402823466e+38f;		// float Max��
const float JOHNHEIGHT = 8.f;

const D3DXVECTOR3	g_vLook(0.f, 0.f, 1000.f);		// �⺻ Look����
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
	
	// ù �̹��� ����
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

	// �Է���ġ �ʱ�ȭ
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
	m_pInput->SetInputStateData();	// ��ġ(���콺, Ű����) �Է��� �����Ѵ�.

	m_pManagement->Update();		// ���� Scene ������Ʈ
}

void CMainApp::Render(void)
{
	float fTime = m_pTimeMgr->GetTime();
	m_pRenderer->Render(fTime);
}

void CMainApp::Release(void)
{
	// System
	Engine::Get_FontMgr()->DestroyInstance();			// ��Ʈ ����̽�
	Engine::Get_Input()->DestroyInstance();				// �Է���ġ
	Engine::Get_GraphicDev()->DestroyInstance();		// �� ��ü
	Engine::Get_TimeMgr()->DestroyInstance();			// Ÿ��


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