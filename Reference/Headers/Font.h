/*!
 * \file Font.h
 * \date 2016/03/29 16:20
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

#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CFont
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice);
	
	/// Block to create by Copy constructor.
	explicit CFont(const CFont& rhs);

public:
	~CFont(void);

public:
	/// Create Font class to safe font style.
	/// Height: Font height.
	/// Width: single word's length.
	/// Weight: how strong font is.
	/// pFaceName: Font name. it will be key value to find a font style.
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice, const UINT& iHeight
		, const UINT& iWidth, const UINT& iWeight, const TCHAR* pFaceName);

public:
	/// Insert parameters to the structure.
	HRESULT InitFont(const UINT& iHeight, const UINT& iWidth, const UINT& iWeight
		, const TCHAR* pFaceName);

	/// Render Insert font.
	/// pString: Words to figure.
	/// Color: Word's color.
	/// vPos: Position of words in Rect
	void Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXFONT				m_pFont;
	UINT					m_iHeight;
	UINT					m_iWidth;
};

END

#endif // Font_h__