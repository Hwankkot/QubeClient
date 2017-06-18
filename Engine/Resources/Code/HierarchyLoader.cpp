#include "HierarchyLoader.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath)
: m_pDevice(pDevice)
, m_pPath(pPath)
{

}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name
												   , LPD3DXFRAME *ppNewFrame)
{
	DERIVED_FRAME*		pNewFrame = new DERIVED_FRAME;
	ZeroMemory(pNewFrame, sizeof(DERIVED_FRAME));

	if(Name != NULL)
	{
		AllocateName(Name, &pNewFrame->Name);
	}

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	pNewFrame->CombindedMatrix = pNewFrame->TransformationMatrix = matIdentity;
	*ppNewFrame = pNewFrame;
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name 
														   , CONST D3DXMESHDATA *pMeshData 
														   , CONST D3DXMATERIAL *pMaterials 
														   , CONST D3DXEFFECTINSTANCE *pEffectInstances 
														   , DWORD NumMaterials 
														   , CONST DWORD *pAdjacency 
														   , LPD3DXSKININFO pSkinInfo 
														   , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	LPD3DXMESH		pMesh = pMeshData->pMesh;

	if(pMesh->GetFVF() == 0)
		return E_FAIL;

	HRESULT		hr = NULL;
	DERIVED_MESHCONTAINER*		pDerivedMeshContainer = new DERIVED_MESHCONTAINER;
	ZeroMemory(pDerivedMeshContainer, sizeof(DERIVED_MESHCONTAINER));

	if(Name != NULL)
		AllocateName(Name, &pDerivedMeshContainer->Name);

	//인접 폴리곤 정보
	DWORD		dwNumFaces = pMesh->GetNumFaces();
	pDerivedMeshContainer->pAdjacency = new DWORD[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwNumFaces * 3);

	//노멀벡터 & 속성정보 셋팅
	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	if(!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, m_pDevice
			, &pDerivedMeshContainer->MeshData.pMesh);
		FAILED_CHECK(hr);

		D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	}
	else
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), m_pDevice
			, &pDerivedMeshContainer->MeshData.pMesh);
		FAILED_CHECK(hr);
		pMesh->AddRef();
	}

	//서브셋
	pDerivedMeshContainer->NumMaterials = max(NumMaterials, 1);
	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	if(NumMaterials > 0)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);
		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			if(pDerivedMeshContainer->pMaterials[i].pTextureFilename == NULL)
				continue;

			TCHAR		szFullPath[MAX_PATH] = L"";
			TCHAR		szFileName[128] = L"";

			lstrcpy(szFullPath, m_pPath);
			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename
				, strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, 128);
			lstrcat(szFullPath, szFileName);

			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[i]);
		}
	}
	else
	{
		ZeroMemory(&pDerivedMeshContainer->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse.a = 0.5f;
		pDerivedMeshContainer->pMaterials[0].pTextureFilename = NULL;
	}

	if(pSkinInfo != NULL)
	{
		pDerivedMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pDerivedMeshContainer->pOriMesh = pMesh;

		DWORD		dwNumBone = pSkinInfo->GetNumBones();
		pDerivedMeshContainer->pFrameOffsetMatrix = new D3DXMATRIX[dwNumBone];
		ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(D3DXMATRIX) * dwNumBone);

		for(DWORD i = 0; i < dwNumBone; ++i)
		{
			pDerivedMeshContainer->pFrameOffsetMatrix[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}
	else
	{
		pDerivedMeshContainer->pSkinInfo = NULL;
		pDerivedMeshContainer->pFrameOffsetMatrix = NULL;
		pDerivedMeshContainer->pOriMesh = NULL;
	}
	*ppNewMeshContainer = pDerivedMeshContainer;
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if(pFrameToFree == NULL)
		return E_FAIL;

	if(pFrameToFree->pMeshContainer != NULL)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if(pFrameToFree->pFrameFirstChild != NULL)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if(pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	Safe_Delete_Array(pFrameToFree->Name);
	Safe_Delete(pFrameToFree);
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	DERIVED_MESHCONTAINER*		pMeshContainer = static_cast<DERIVED_MESHCONTAINER*>(pMeshContainerToFree);
	for(DWORD i = 0; i < pMeshContainerToFree->NumMaterials; ++i)
	{
		Safe_Release(pMeshContainer->ppTexture[i]);
	}
	Safe_Delete_Array(pMeshContainer->ppTexture);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pMeshContainer->ppFrameConbinedMatrix);
	Safe_Delete_Array(pMeshContainer->pResultMatrix);
	Safe_Delete_Array(pMeshContainer->pAdjacency);

	Safe_Release(pMeshContainer->pOriMesh);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete_Array(pMeshContainer);
	return S_OK;
}

void Engine::CHierarchyLoader::AllocateName(const CHAR* const pName, CHAR** ppNewName)
{
	int		iLength = strlen(pName)	;

	*ppNewName = new CHAR[iLength + 1];

	strcpy_s(*ppNewName, iLength + 1, pName);
}

