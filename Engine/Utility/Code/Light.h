/*!
 * \file Light.h
 * \date 2016/03/25 16:21
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

#ifndef Light_h__
#define Light_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL  CLight
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pDevice);
	explicit CLight(const CLight&);
	CLight& operator = (const CLight&);

public:
	~CLight(void);

public:
	/// Create CLight Object and call InitLight Method.
	static CLight* Create(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightState, const DWORD& dwLightIdx);

public:
	/// Set Light and Light Enable.
	HRESULT InitLight(const D3DLIGHT9* pLightState, const DWORD& dwLightIdx);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DLIGHT9				m_LightState;
};

END

#endif // Light_h__