/*!
 * \file FontMgr.h
 * \date 2016/03/29 16:44
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

#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

using namespace std;

BEGIN(Engine)

class CFont;

class ENGINE_DLL CFontMgr
{
public:
	DECLARE_SINGLETON(CFontMgr)

private:
	CFontMgr(void);
	~CFontMgr(void);

public:
	/// Create CFont class and add to map to save a font style.
	HRESULT AddFont(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey
		, const UINT& iHeight, const UINT& iWidth, const UINT& iWeight);

	/// Render pString by options.
	/// pFontKey: Font name.
	/// vPos: Font Position of Rect.
	/// Color: Font Color.
	void Render(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color);

private:
	void Release(void);

private:
	typedef map<const TCHAR*, CFont*>		MAPFONT;
	MAPFONT m_mapFont;
};

END

#endif // FontMgr_h__