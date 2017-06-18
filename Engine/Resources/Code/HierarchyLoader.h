/*!
 * \file HierarchyLoader.h
 * \date 2016/04/07 16:09
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

#ifndef HierarchyLoader_h__
#define HierarchyLoader_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyLoader
	: public ID3DXAllocateHierarchy
{
public:
	CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath);
	virtual ~CHierarchyLoader(void);

public:
	virtual STDMETHODIMP CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	virtual STDMETHODIMP CreateMeshContainer(THIS_ LPCSTR Name
		, CONST D3DXMESHDATA *pMeshData
		, CONST D3DXMATERIAL *pMaterials
		, CONST D3DXEFFECTINSTANCE *pEffectInstances
		, DWORD NumMaterials
		, CONST DWORD *pAdjacency
		, LPD3DXSKININFO pSkinInfo
		, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	virtual STDMETHODIMP DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree);
	virtual STDMETHODIMP DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	void AllocateName(const CHAR* const pName, CHAR** ppNewName);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	const TCHAR*			m_pPath;
};

END

#endif // HierarchyLoader_h__