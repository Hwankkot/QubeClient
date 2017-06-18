#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"
#include "CubeColor.h"
#include "Engine_Function.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_pLoader(NULL)
, m_pRootBone(NULL)
, m_pAniCtrl(NULL)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
: CMesh(rhs.m_pDevice)
, m_pLoader(rhs.m_pLoader)
, m_pRootBone(rhs.m_pRootBone)
{
	m_pAniCtrl = new CAniCtrl(*(rhs.m_pAniCtrl));
	m_pBoundingBox = rhs.m_pBoundingBox;
	m_StdVtx3 = rhs.m_StdVtx3;
	m_Material = rhs.m_Material;
	++m_dwRefCnt;
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

DOUBLE Engine::CDynamicMesh::GetTrackPosition(void)
{
	// 현재 실행 중인 애니메이션이 어느 위치인지 반환 
	return m_pAniCtrl->GetTrackPosition();
}

DOUBLE Engine::CDynamicMesh::GetPeriod(void)
{
	// 전체 트랙 크기
	return m_pAniCtrl->GetPeriod();
}

void Engine::CDynamicMesh::SetTrackPos(DOUBLE dTrackPos)
{
	
	m_pAniCtrl->SetTrackPos(dTrackPos);
}

Engine::CResource* Engine::CDynamicMesh::CloneResource(void)
{
	return new CDynamicMesh(*this);
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	TCHAR		szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = new CHierarchyLoader(m_pDevice, pPath);
	m_pAniCtrl = new CAniCtrl;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(szFullPath
		, D3DXMESH_MANAGED
		, m_pDevice
		, m_pLoader
		, NULL
		, &m_pRootBone
		, m_pAniCtrl->GetAniCtrl());	
	FAILED_CHECK(hr);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBone, &matIdentity);
	SetUpFrameMatrixPointer((DERIVED_FRAME*)m_pRootBone);

	m_Material.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Material.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Material.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	/*MakeBoundingBox((DERIVED_FRAME*)m_pRootBone, m_StdVtx3._min, m_StdVtx3._max);
	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_StdVtx3, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));*/
	return S_OK;
}

void Engine::CDynamicMesh::RenderMesh(const D3DXMATRIX* pWorldMatrix)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);
	Find_MeshContainer((DERIVED_FRAME*)m_pRootBone);

//#ifdef _DEBUG
//	m_pDevice->SetTexture(0, NULL);
//	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	m_pBoundingBox->Render(pWorldMatrix);
//	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//#endif
}

DWORD Engine::CDynamicMesh::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		m_pLoader->DestroyFrame(m_pRootBone);
		Safe_Delete(m_pLoader);
		Safe_Delete(m_pAniCtrl);
		//Safe_Delete(m_pBoundingBox);
		return 0;
	}
	else
	{
		Safe_Delete(m_pAniCtrl);
		--m_dwRefCnt;
	}
	return m_dwRefCnt;
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath , const TCHAR* pFileName)
{
	CDynamicMesh*		pMesh = new CDynamicMesh(pDevice);
	
	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
		Safe_Delete(pMesh);

	return pMesh;
}

void Engine::CDynamicMesh::UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix)
{
	if(pFrame != NULL)
		pFrame->CombindedMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if(pFrame->pFrameFirstChild != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameFirstChild, &pFrame->CombindedMatrix);

	if(pFrame->pFrameSibling != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameSibling, pParentMatrix);
}

void Engine::CDynamicMesh::SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
		return;

	if(pFrame->pMeshContainer != NULL)
	{
		if(pFrame->pMeshContainer->pSkinInfo == NULL)
			return;
		 
		DWORD dwNumFrame = pFrame->pMeshContainer->pSkinInfo->GetNumBones();
		((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameConbinedMatrix = new D3DXMATRIX*[dwNumFrame];
		ZeroMemory(((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameConbinedMatrix, sizeof(D3DXMATRIX*) * dwNumFrame);

		for(DWORD i = 0; i < dwNumFrame; ++i)
		{
			((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameConbinedMatrix[i]
			= &((DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrame->pMeshContainer->pSkinInfo->GetBoneName(i)))->CombindedMatrix;
		}
	}

	if(pFrame->pFrameFirstChild != NULL)
		SetUpFrameMatrixPointer((DERIVED_FRAME*)pFrame->pFrameFirstChild);

	if(pFrame->pFrameSibling != NULL)
		SetUpFrameMatrixPointer((DERIVED_FRAME*)pFrame->pFrameSibling);
}

void Engine::CDynamicMesh::Find_MeshContainer(DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
		return;

	if(pFrame->pMeshContainer != NULL)
		Render_MeshContainer((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer);

	if(pFrame->pFrameFirstChild != NULL)
		Find_MeshContainer((DERIVED_FRAME*)pFrame->pFrameFirstChild);

	if(pFrame->pFrameSibling != NULL)
		Find_MeshContainer((DERIVED_FRAME*)pFrame->pFrameSibling);
}

void Engine::CDynamicMesh::Render_MeshContainer(DERIVED_MESHCONTAINER* pMeshContainer)
{
	if(pMeshContainer->pSkinInfo != NULL)
	{
		DWORD		dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();
		if(pMeshContainer->pResultMatrix == NULL)
			pMeshContainer->pResultMatrix = new D3DXMATRIX[dwNumBones];

		for(DWORD i = 0; i < dwNumBones; ++i)
		{
			pMeshContainer->pResultMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameConbinedMatrix[i]);
		}

		BYTE*		pSour;
		BYTE*		pDest;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, (void**)&pSour);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (void**)&pDest);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pResultMatrix
			, NULL
			, pSour
			, pDest);

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for(DWORD i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			m_pDevice->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

void Engine::CDynamicMesh::SetAnimationSet(UINT iAniIdx, DOUBLE dAccTime)
{
	m_pAniCtrl->SetAnimationSet(iAniIdx, dAccTime);
}

void Engine::CDynamicMesh::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->FrameMove(fTimePerSec);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBone, &matIdentity);
}

const D3DXMATRIX* Engine::CDynamicMesh::FindFrame(const char* pFrameName)
{
	DERIVED_FRAME*		pFrmae = ((DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrameName));

	return &pFrmae->CombindedMatrix;
}

void Engine::CDynamicMesh::MakeBoundingBox(D3DXFRAME* pFrame, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	if(pFrame->pMeshContainer != NULL)
	{
		LPD3DXMESH pMesh =  pFrame->pMeshContainer->MeshData.pMesh;
		if(pMesh != NULL)
		{
			DWORD	dwFVF = pMesh->GetFVF();
			if(dwFVF & D3DFVF_XYZ)
			{
				D3DXVECTOR3		vTempMin = D3DXVECTOR3(0.f, 0.f, 0.f);
				D3DXVECTOR3		vTempMax = D3DXVECTOR3(0.f, 0.f, 0.f);

				D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];
				pMesh->GetDeclaration(VtxFVF);

				int		iIndex = 0;
				DWORD	dwOffset = 0;

				while(true)
				{
					if(VtxFVF[iIndex].Usage == D3DDECLUSAGE_POSITION)
					{
						dwOffset = VtxFVF[iIndex].Offset;
						break;
					}
					++iIndex;
				}

				void*		pVtxState = NULL;

				pMesh->LockVertexBuffer(0,  &pVtxState);

				BYTE*		pByteVtxState = (BYTE*)pVtxState;
				pVtxState = &pByteVtxState[dwOffset];

				DWORD	dwNumVertices = pMesh->GetNumVertices();
				DWORD	dwStride = D3DXGetFVFVertexSize(dwFVF);

				D3DXComputeBoundingBox((D3DXVECTOR3*)pVtxState, dwNumVertices, dwStride, &vTempMin, &vTempMax);

				pMesh->UnlockVertexBuffer();

				vMax.x = max(vTempMax.x, vMax.x);
				vMax.y = max(vTempMax.y, vMax.y);
				vMax.z = max(vTempMax.z, vMax.z);

				vMin.x = min(vTempMin.x, vMin.x);
				vMin.y = min(vTempMin.y, vMin.y);
				vMin.z = min(vTempMin.z, vMin.z);
			}
		}
	}

	if(pFrame->pFrameFirstChild != NULL)
		MakeBoundingBox(pFrame->pFrameFirstChild, vMin, vMax);

	if(pFrame->pFrameSibling != NULL)
		MakeBoundingBox(pFrame->pFrameSibling, vMin, vMax);
}
