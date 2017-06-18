/*!
 * \file GameObject.h
 * \date 2015/04/03 18:11
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
#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	
private:
	explicit CGameObject(const CGameObject&);
	CGameObject& operator = (const CGameObject&);

public:
	virtual ~CGameObject(void);

public:
	virtual void Update(void);
	virtual void Render(void) {}

protected:
	/// 현재 카메라의 위치(Dynamic)에서부터 Z버퍼까지의 거리를 구해 저장한다.
	void Compute_ViewZ(const D3DXVECTOR3& vPos);

public:
	const float& Get_ViewZ(void) const;

private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	float m_fViewZ; // Z 버퍼 크기

protected:
	typedef std::map<DWORD, CComponent*> MAPCOMPONENT;
	MAPCOMPONENT m_mapComponent;
};

END

#endif // GameObject_h__