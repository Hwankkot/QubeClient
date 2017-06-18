#include "StaticMesh.h"

#include "CubeColor.h"
#include "Engine_Function.h"
#include "VIBuffer.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_ppTexture(NULL)
, m_pMtrl(NULL)
, m_pMesh(NULL)
, m_pSubset(NULL)
, m_dwSubsetCnt(0)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: CMesh(rhs.m_pDevice)
, m_ppTexture(rhs.m_ppTexture)
, m_pMtrl(rhs.m_pMtrl)
, m_pMesh(rhs.m_pMesh)
, m_pSubset(rhs.m_pSubset)
, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
{
	m_Material = rhs.m_Material;
	m_StdVtx3 = rhs.m_StdVtx3;
	m_pBoundingBox = rhs.m_pBoundingBox;
}

Engine::CStaticMesh::~CStaticMesh(void)
{
	
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CStaticMesh* pMesh = new CStaticMesh(pDevice);
	

	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
		Safe_Delete(pMesh);

	pMesh->MakeBoundingBox();

	return pMesh;
}

Engine::CResource* Engine::CStaticMesh::CloneResource(void)
{
	CStaticMesh* pMesh = new CStaticMesh(*this);
	
	pMesh->AddRefCnt();

	return pMesh;
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT  hr = NULL;
	TCHAR szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	/// 메시 파일 내의 서브셋의 개수와 정보를 받아온다. 
	/// szFullPath: 메시 파일의 경로 + 이름 
	/// D3DMESH_MANAGED: 메시 생성 옵션, 메시 데이터 관리 방법
	/// m_pDevice: 메시와 연결할 디바이스 주소
	/// m_pSubset: 서브셋 주소를 받을 포인터
	/// m_dwSubSetCnt: 서브셋 개수
	/// m_pMesh: 메쉬 정보를 받을 포인터
	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice, NULL, &m_pSubset, NULL
		, &m_dwSubsetCnt, &m_pMesh);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, szFullPath);

	// 서브셋 포인터를 D3DXMATERIAL(서브셋 구조체)의 포인터 형태로 변환한다.
	D3DXMATERIAL* pSubset = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	//// 서브셋의 재질 정보(D3DMATERIAL9)을 담을 구조체 배열을 만든다.
	m_pMtrl = new D3DMATERIAL9[m_dwSubsetCnt];
	//// Texture Com 객체를 담을 포인터 배열을 만든다.
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pMtrl[i] = pSubset->MatD3D;

		TCHAR  szFileName[128] = L"";

		lstrcpy(szFullPath, pPath);
		
		// 인자값으로 받은 MultiByteChar를 WideChar형으로 변환한다. 

		if(pSubset->pTextureFilename != NULL)
		{
			MultiByteToWideChar(CP_ACP
				, 0, pSubset[i].pTextureFilename
				, strlen(pSubset[i].pTextureFilename)
				, szFileName, 128);
			lstrcat(szFullPath, szFileName);

			// Texture com 객체를 생성하고 포인터를 담는다.
			hr = D3DXCreateTextureFromFileEx(m_pDevice, szFullPath, 0, 0, 0, D3DUSAGE_RENDERTARGET, D3DFMT_UNKNOWN
				, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_ppTexture[i]);
			//hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
			FAILED_CHECK(hr);
		}
	}

	m_Material.Diffuse	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Material.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Material.Ambient	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Material.Emissive = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
	m_Material.Power = 0.2f;
	
	return S_OK;
}

void Engine::CStaticMesh::RenderMesh(const D3DXMATRIX* pWorldMatrix)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pDevice->SetTexture(0, m_ppTexture[i]);
		m_pDevice->SetMaterial(&m_Material);
		m_pMesh->DrawSubset(i);
	}
	//m_pBoundingBox->Render(pWorldMatrix);
}

DWORD Engine::CStaticMesh::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);
		
		for(DWORD i = 0; i < m_dwRefCnt; ++i)
		{
			/// Release texture objects.
			Safe_Release(m_ppTexture[i]);
		}

		/// Delete pointer array.
		Safe_Delete_Array(m_ppTexture);
		Safe_Delete(m_pBoundingBox);

		Safe_Release(m_pMesh);
		Safe_Release(m_pSubset);
		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

void Engine::CStaticMesh::MakeBoundingBox(void)
{
	/// 메시의 버텍스 포멧 타입을 불러와서 저장한다.
	DWORD	dwFVF = m_pMesh->GetFVF();

	/// 안에 XYZ정보가 들어있는지 확인한다. 
	if(dwFVF & D3DFVF_XYZ)
	{
		/// 파이프라인의 입력 정점 데이터를 정의하는 구조체를 저장한다.
		D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];

		/// 메시에 포함되어있는 정점의 선언들을 불러온다. 
		m_pMesh->GetDeclaration(VtxFVF);

		int		iIndex = 0;
		DWORD	dwOffset = 0;

		while(true)
		{
			/// 메시의 포지션 정보가 들어있는 오프셋을 찾아서 저장한다.
			if(VtxFVF[iIndex].Usage == D3DDECLUSAGE_POSITION)
			{
				dwOffset = VtxFVF[iIndex].Offset;
				break;
			}
			++iIndex;
		}

		void* pVtxState = NULL;

		m_pMesh->LockVertexBuffer(0,  &pVtxState);

		BYTE*		pByteVtxState = (BYTE*)pVtxState;
		pVtxState = &pByteVtxState[dwOffset];

		/// 메시 안의 버텍스의 개수를 불러온다.
		DWORD	dwNumVertices = m_pMesh->GetNumVertices();

		/// 메시 안의 버텍스 포멧 정보를 불러온다.
		DWORD	dwStride = D3DXGetFVFVertexSize(dwFVF);

		/// 바운딩 박스를 구성할 Min, Max 정보를 저장한다.
		D3DXComputeBoundingBox((D3DXVECTOR3*)pVtxState, dwNumVertices, dwStride, &m_StdVtx3._min, &m_StdVtx3._max);

		m_pMesh->UnlockVertexBuffer();

		m_pBoundingBox = CCubeColor::Create(m_pDevice, m_StdVtx3, D3DXCOLOR(0.f,  1.f, 0.f, 1.f));
	}
}

LPD3DXMESH Engine::CStaticMesh::GetMesh(void)
{
	return m_pMesh;
}
