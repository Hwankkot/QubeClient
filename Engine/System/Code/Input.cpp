#include "Input.h"

#include "Engine_Function.h"

//-------------------------------------------------------
// 전역변수 (입력 키 값)
//-------------------------------------------------------

const DWORD Engine::CInput::KEY_LBUTTON		= 0x00000001;
const DWORD Engine::CInput::KEY_RBUTTON		= 0x00000002;
const DWORD Engine::CInput::KEY_MBUTTON		= 0x00000004;
const DWORD Engine::CInput::KEY_ENTER		= 0x00000008;
const DWORD Engine::CInput::KEY_SPACE		= 0x00000010;
const DWORD Engine::CInput::KEY_SHIFT		= 0x00000020;
const DWORD Engine::CInput::KEY_W			= 0x00000040;
const DWORD Engine::CInput::KEY_S			= 0x00000080;
const DWORD Engine::CInput::KEY_A			= 0x00000100;
const DWORD Engine::CInput::KEY_D			= 0x00000200;

//-------------------------------------------------------

IMPLEMENT_SINGLETON(Engine::CInput)

Engine::CInput::CInput(void)
: m_pInput(NULL)
, m_pKeyBoard(NULL)
, m_pMouse(NULL)
{
	ZeroMemory(m_byKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInput::~CInput(void)
{
	Release();
}

BYTE Engine::CInput::GetDIKeyState(BYTE KeyFlag)
{
	return m_byKeyState[KeyFlag];
}

BYTE Engine::CInput::GetDIMouseState(MOUSECLICK KeyFlag)
{
	return m_MouseState.rgbButtons[KeyFlag];
}

long Engine::CInput::GetDIMouseMove(MOUSEMOVE KeyFlag)
{
	return	*(((long*)&m_MouseState) + KeyFlag);
}

void Engine::CInput::SetInputStateData(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pKeyBoard->Acquire();
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
	m_pMouse->Acquire();
}

HRESULT Engine::CInput::InitInputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK(hr);

	hr = InitKeyBoard(hWnd);
	FAILED_CHECK(hr);

	hr = InitMouse(hWnd);
	FAILED_CHECK(hr);
	return S_OK;
}

HRESULT Engine::CInput::InitKeyBoard(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	FAILED_CHECK(hr);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();
	return S_OK;
}

HRESULT Engine::CInput::InitMouse(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	FAILED_CHECK(hr);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();
	return S_OK;
}

void Engine::CInput::Release(void)
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInput);
}