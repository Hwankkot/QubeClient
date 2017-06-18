/*!
 * \file GraphicDev.cpp
 * \date 2015/02/25 11:45
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

#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev
{
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit CGraphicDev(void);
public:
	~CGraphicDev(void);

public:
	/// 3DDevice9 Getter
	inline LPDIRECT3DDEVICE9 GetDevice(void);

public:
	/// Create Direct com object.
	HRESULT InitGraphicDev(WINMODE eWinMode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);

private:
	/// Set D3DPRESENT_PARAMETERS Structure
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const WINMODE& eWinMode
					  , HWND hWnd, const WORD& wSizeX, const WORD& wSizeY); 
	void Release(void);

private:
	LPDIRECT3D9					m_pSDK;    /// Device Com Object
	LPDIRECT3DDEVICE9			m_pDevice; /// Direct3DDevice9 Variable
};

END

#endif // GraphicDev_h__
