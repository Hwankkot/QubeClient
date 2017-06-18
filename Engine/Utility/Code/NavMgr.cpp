#include "NavMgr.h"
#include "NavCell.h"

#include "Engine_Functor.h"
#include "Line2D.h"

IMPLEMENT_SINGLETON(Engine::CNavMgr)

Engine::CNavMgr::CNavMgr(void)
: m_pDevice(NULL)
, m_pLine(NULL)
, m_dwReservedSize(0)
, m_dwIdxCnt(0)
{

}

Engine::CNavMgr::~CNavMgr(void)
{
	Release();
}

HRESULT Engine::CNavMgr::InitNavMeshManager(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwContainerSize)
{
	m_pDevice = pDevice;
	m_dwReservedSize = dwContainerSize;
	m_NavMesh.reserve(m_dwReservedSize);
	FAILED_CHECK(D3DXCreateLine(m_pDevice, &m_pLine));

	return S_OK;
}

HRESULT Engine::CNavMgr::AddCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
{
	CNavCell* pNavCell = CNavCell::Create(m_pDevice, pPointA, pPointB, pPointC, m_dwIdxCnt);
	NULL_CHECK_RETURN(pNavCell, E_FAIL);

	// 다음 셀의 Index.
	++m_dwIdxCnt;

	m_NavMesh.push_back(pNavCell);

	return S_OK;
}

void Engine::CNavMgr::LinkCell(void)
{
	NAVMESH::iterator	iter = m_NavMesh.begin();
	NAVMESH::iterator	iter_end = m_NavMesh.end();

	if(iter == iter_end)
		return;

	for(; iter != iter_end; ++iter)
	{
		NAVMESH::iterator	iter_Target = m_NavMesh.begin();
		while(iter_Target != iter_end)
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if((*iter_Target)->Compare_Point((*iter)->GetPoint(CNavCell::POINT_A), (*iter)->GetPoint(CNavCell::POINT_B), (*iter)))
				(*iter)->SetNeighbor(CNavCell::NEIGHBOR_AB, (*iter_Target));

			else if((*iter_Target)->Compare_Point((*iter)->GetPoint(CNavCell::POINT_B), (*iter)->GetPoint(CNavCell::POINT_C), (*iter)))
				(*iter)->SetNeighbor(CNavCell::NEIGHBOR_BC, (*iter_Target));

			else if((*iter_Target)->Compare_Point((*iter)->GetPoint(CNavCell::POINT_C), (*iter)->GetPoint(CNavCell::POINT_A), (*iter)))
				(*iter)->SetNeighbor(CNavCell::NEIGHBOR_CA, (*iter_Target));
			++iter_Target;
		}
	}
}

void Engine::CNavMgr::RenderNavMesh(void)
{
	size_t iNavMeshSize = m_NavMesh.size();

	if(iNavMeshSize == 0)
		return ;

	for(size_t i = 0; i < iNavMeshSize; ++i)
		m_NavMesh[i]->Render(m_pLine);
}

void Engine::CNavMgr::Release(void)
{
	for_each(m_NavMesh.begin(), m_NavMesh.end(), CDeleteObj());
	m_NavMesh.clear();
}

DWORD Engine::CNavMgr::MoveOnNavMesh(D3DXVECTOR3* pEye, D3DXVECTOR3* pAt, D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx)
{

	CNavCell::NEIGHBOR			eNeighbor;
	DWORD		dwNextIndex = dwCurrentIdx;
	//라인을 지나갔을때
	if(m_NavMesh[dwCurrentIdx]->IsPassed(*pEye, *pDir, &eNeighbor))
	{
		CNavCell*		pNeighbor = m_NavMesh[dwCurrentIdx]->GetNeighbor(eNeighbor);
		//이웃이 존재하지 않을때
		if(pNeighbor == NULL)
		{
			//슬라이딩 백터
		}
		else //이웃이 존재 할때
		{
			dwNextIndex = pNeighbor->GetIndex();
			*pEye += *pDir;
			*pAt += *pDir;
		}
	}
	else //지나가지 않았을때
	{
		*pEye += *pDir;
		*pAt += *pDir;
	}
	return dwNextIndex;

}	

std::vector<Engine::CNavCell*>* Engine::CNavMgr::GetNavMesh(void)
{
	return &m_NavMesh;
}
