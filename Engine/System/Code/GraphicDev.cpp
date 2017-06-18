#include "GraphicDev.h"

#include "Engine_Function.h"

IMPLEMENT_SINGLETON(Engine::CGraphicDev)

Engine::CGraphicDev::CGraphicDev(void)
: m_pSDK(NULL)
, m_pDevice(NULL)
{

}

Engine::CGraphicDev::~CGraphicDev(void)
{
	Release();
}

LPDIRECT3DDEVICE9 Engine::CGraphicDev::GetDevice(void)
{
	return m_pDevice;
}

HRESULT Engine::CGraphicDev::InitGraphicDev(WINMODE eWinMode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	HRESULT hr = NULL;

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 d3dcpas9;
	ZeroMemory(&d3dcpas9, sizeof(D3DCAPS9));

	hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcpas9);
	FAILED_CHECK_MSG(hr, L"Failed to Create Device");

	DWORD vp;

	if(d3dcpas9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	/// Set MultiThread
	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, eWinMode, hWnd, wSizeX, wSizeY);

	hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice);
	FAILED_CHECK_MSG(hr, L"Failed to Create 3DDevice9");	

	return S_OK;
}

void Engine::CGraphicDev::SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const WINMODE& eWinMode
									  , HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	d3dpp.BackBufferWidth = wSizeX;
	d3dpp.BackBufferHeight = wSizeY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eWinMode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void Engine::CGraphicDev::Release(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}
