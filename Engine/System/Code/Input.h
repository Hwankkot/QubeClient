/*!
 * \file Input.h
 * \date 2016/03/30 15:21
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Input_h__
#define Input_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CInput
{
public:
	DECLARE_SINGLETON(CInput)

public:
	enum MOUSECLICK {DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON,};
	enum MOUSEMOVE {DIM_X, DIM_Y, DIM_Z};


public:
	static const DWORD KEY_LBUTTON;
	static const DWORD KEY_RBUTTON;	
	static const DWORD KEY_MBUTTON;	
	static const DWORD KEY_ENTER;	
	static const DWORD KEY_SPACE;	
	static const DWORD KEY_SHIFT;	
	static const DWORD KEY_W;		
	static const DWORD KEY_S;		
	static const DWORD KEY_A;		
	static const DWORD KEY_D;

private:
	CInput(void);
	~CInput(void);


public:
	HRESULT InitInputDevice(HINSTANCE hInst, HWND hWnd);
private:
	HRESULT InitKeyBoard(HWND hWnd);
	HRESULT InitMouse(HWND hWnd);
	void Release(void);

public:
	void SetInputStateData(void);
	BYTE GetDIKeyState(BYTE KeyFlag);
	BYTE GetDIMouseState(MOUSECLICK KeyFlag);
	long GetDIMouseMove(MOUSEMOVE KeyFlag);


private:
	LPDIRECTINPUT8				m_pInput;
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

private:
	BYTE				m_byKeyState[256];
	DIMOUSESTATE		m_MouseState;
};

END

#endif // Input_h__
