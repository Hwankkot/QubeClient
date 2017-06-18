/*!
 * \file LightMgr.h
 * \date 2016/03/25 16:27
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

#ifndef LightMgr_h__
#define LightMgr_h__

/// Engine
#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CLight;

class ENGINE_DLL CLightMgr
{
public:
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr(void);
	~CLightMgr(void);

public:
	/// Add Light Object.
	/// Create CLight and Copy pLightState by memcpy.
	/// Set dwLightIdx to device.
	HRESULT AddLight(LPDIRECT3DDEVICE9 pDevice , const D3DLIGHT9* pLightState , const DWORD& dwLightIdx);

public:
	/// Delete list
	void Release(void);

private:
	typedef std::list<CLight*> LIGHTLIST;
	LIGHTLIST m_Lightlist;
};

END

#endif // LightMgr_h__