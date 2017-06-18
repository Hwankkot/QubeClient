/*!
 * \file ResourceMgr.h
 * \date 2015/04/03 16:11
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
#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Enum.h"
#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class CResource;

class ENGINE_DLL CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)

private:
	CResourceMgr(void);
	~CResourceMgr(void);

public:
	CResource* FindResource(const DWORD& dwResourceKey);
	CResource* CloneResource(const DWORD& dwResourceKey);

public:
	HRESULT AddBuffer(LPDIRECT3DDEVICE9 pDevice
		, BUFFERTYPE eBufferType
		, const DWORD& dwResourceKey
		, const float& fRadius = 0.f
		, const STANDARD_VERTEX3& StdVtx3 = STANDARD_VERTEX3(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f))
		, const DWORD& dwColor = 0);

	HRESULT	AddMesh(LPDIRECT3DDEVICE9 pDevice
		, MESHTYPE eMeshType
		, const DWORD& dwResourceKey
		, const TCHAR* pFilePath
		, const TCHAR* pFileName);

	HRESULT AddTexture(LPDIRECT3DDEVICE9 pDevice
		, TEXTURETYPE eTextureType
		, const DWORD& dwResourceKey
		, const TCHAR* pFilePath
		, const WORD& wCnt = 1);

private:
	void Release(void);

private:
	typedef std::map<DWORD, CResource*>  MAPRESOURCE;
	MAPRESOURCE	m_mapResource;
};

END

#endif // ResourceMgr_h__