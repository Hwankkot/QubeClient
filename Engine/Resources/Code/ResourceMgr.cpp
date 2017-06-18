#include "ResourceMgr.h"

#include "CubeColor.h"
#include "DynamicMesh.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "RcTex.h"
#include "Resource.h"
#include "StaticMesh.h"
#include "SphereColor.h"
#include "Texture.h"
#include "VIBuffer.h"

IMPLEMENT_SINGLETON(Engine::CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
{

}

Engine::CResourceMgr::~CResourceMgr(void)
{
	Release();
}

Engine::CResource* Engine::CResourceMgr::FindResource(const DWORD& dwResourceKey)
{
	int iResult = m_mapResource.count(dwResourceKey);
	
	if(iResult == NULL)
		return NULL;
	
	return m_mapResource[dwResourceKey];
}

Engine::CResource* Engine::CResourceMgr::CloneResource(const DWORD& dwResourceKey)
{
	int iResult = m_mapResource.count(dwResourceKey);

	if(iResult == NULL)
		return NULL;	

	return m_mapResource[dwResourceKey]->CloneResource();
}

HRESULT Engine::CResourceMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice
										, BUFFERTYPE eBufferType
										, const DWORD& dwResourceKey
										, const float& fRadius /* = 0.f*/
										, const STANDARD_VERTEX3& StdVtx3 /*= StdVtx3(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f))*/
										, const DWORD& dwColor /* = 0 */)
{
	/// 이미 존재하는 리소스인지 확인
	CResource* pResource = FindResource(dwResourceKey);
	EXIST_CHECK_RETURN_TAGMSG(pResource, E_FAIL, L"AddBuffer",  L"The buffer already exists in The ResourceManager container.");

	CVIBuffer* pBuffer = NULL;

	switch(eBufferType)
	{
	case BUFFER_RCTEX:
		pBuffer = CRcTex::Create(pDevice);
		NULL_CHECK_RETURN(pBuffer, E_FAIL);
		break;

	case BUFFER_CUBECOL:
		pBuffer = CCubeColor::Create(pDevice, StdVtx3, dwColor);
		NULL_CHECK_RETURN(pBuffer, E_FAIL);
		break;

	case BUFFER_SPHERECOL:
		pBuffer = CSphereColor::Create(pDevice, fRadius);
		NULL_CHECK_RETURN(pBuffer, E_FAIL);
		break;
	}

	m_mapResource.insert(MAPRESOURCE::value_type(dwResourceKey, pBuffer));

	return S_OK;
}

HRESULT Engine::CResourceMgr::AddMesh(LPDIRECT3DDEVICE9 pDevice
									  , MESHTYPE eMeshType 
									  , const DWORD& dwResourceKey
									  , const TCHAR* pFilePath 
									  , const TCHAR* pFileName)
{
	CResource* pResource = FindResource(dwResourceKey);
	NULL_CHECK_RETURN_MSG(!pResource, E_FAIL, L"The Mesh already exists.");

	switch(eMeshType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pDevice, pFilePath, pFileName);
		break;

	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pDevice, pFilePath, pFileName);
		break;
	}
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource.insert(MAPRESOURCE::value_type(dwResourceKey, pResource));
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice
										 , TEXTURETYPE eTextureType
										 , const DWORD& dwResourceKey
										 , const TCHAR* pFilePath
										 , const WORD& wCnt /*= 1*/)
{
	CResource*		pResource = FindResource(dwResourceKey);
	EXIST_CHECK_RETURN_TAGMSG(pResource, E_FAIL, L"AddTexture", L"The texture already exists.")

	pResource = CTexture::Create(pDevice, eTextureType, pFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, L"Failed to allocate the resource.");
	
	m_mapResource.insert(MAPRESOURCE::value_type(dwResourceKey, pResource));
	
	return S_OK;
}

void Engine::CResourceMgr::Release(void)
{
	MAPRESOURCE::iterator iter = m_mapResource.begin();
	MAPRESOURCE::iterator iter_end = m_mapResource.end();

	DWORD dwRefCnt = 0;

	for(; iter != iter_end; ++iter)
	{
		dwRefCnt = iter->second->Release();
	
		if(dwRefCnt == 0)
			Safe_Delete(iter->second);
	}

	m_mapResource.clear();

}