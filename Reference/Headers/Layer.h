/*!
 * \file Layer.h
 * \date 2016/04/05 13:01
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

#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CLayer
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CLayer(const CLayer&);
	CLayer& operator = (const CLayer&);

public:
	~CLayer(void);

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);
	void Update(void);
	void Render(void);
	void Release(void);

public:
	void AddObject(const DWORD& dwObjectKey, CGameObject* pGameObject);
	void RemoveObject(const DWORD& dwObjectKey, CGameObject* pGameObject);

public:
	void SetMapObjectlist(std::map<DWORD, std::vector<CGameObject*>>* pmapObjectlist);
	std::map<DWORD, std::vector<CGameObject*>>* GetMapObjectlist(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	
private:
	typedef std::vector<CGameObject*>	OBJECTLIST;
	typedef std::map<DWORD, OBJECTLIST>	OBJECTLISTMAP;
	OBJECTLISTMAP m_mapObjectlist;
};

END

#endif // Layer_h__