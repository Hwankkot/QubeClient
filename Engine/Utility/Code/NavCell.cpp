#include "NavCell.h"

#include "Engine_Function.h"
#include "Line2D.h"

Engine::CNavCell::CNavCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
: m_pDevice(NULL)
, m_dwIndex(0)
{
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	ZeroMemory(m_pNeighbor, sizeof(CNavCell*) * NEIGHBOR_END);
 }

Engine::CNavCell::~CNavCell(void)
{
	Release();
}

Engine::CNavCell* Engine::CNavCell::Create(LPDIRECT3DDEVICE9 pDevice
										   , const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC , const DWORD& dwIdx)
{

	CNavCell* pNavCell = new CNavCell(pPointA, pPointB, pPointC);

	if(FAILED(pNavCell->InitNavCell(pDevice, dwIdx)))
		Safe_Delete(pNavCell);

	return pNavCell;
}

HRESULT Engine::CNavCell::InitNavCell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx)
{
	m_pDevice = pDevice;
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	m_dwIndex = dwIdx;

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);

	return S_OK;
}

const D3DXVECTOR3* Engine::CNavCell::GetPoint(POINT ePointID)
{
	return &m_vPoint[ePointID];
}

DWORD Engine::CNavCell::GetIndex(void)
{
	return m_dwIndex;
}

void Engine::CNavCell::SetNeighbor(NEIGHBOR eNeighberID, CNavCell* pNeighbor)
{
	m_pNeighbor[eNeighberID] = pNeighbor;
}

Engine::CNavCell* Engine::CNavCell::GetNeighbor(NEIGHBOR eNeighborID)
{
	return m_pNeighbor[eNeighborID];
}

bool Engine::CNavCell::Compare_Point(const D3DXVECTOR3* pPointStart, const D3DXVECTOR3* pPointEnd, CNavCell* pNeighbor)
{
	if(*pPointStart == m_vPoint[POINT_A])
	{
		if(*pPointEnd == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pPointEnd == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
	}

	if(*pPointStart == m_vPoint[POINT_B])
	{
		if(*pPointEnd == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pPointEnd == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	if(*pPointStart == m_vPoint[POINT_C])
	{
		if(*pPointEnd == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else if(*pPointEnd == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	return false;
}

void Engine::CNavCell::Render(LPD3DXLINE pLine)
{
	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3 vPoint[4];

	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];

	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if(vPoint[i].z < 0.f)
			vPoint[i].z = 0.f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX  matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	pLine->SetWidth(2.f);
	pLine->Begin();
	pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	pLine->End();
	
}

void Engine::CNavCell::Release(void)
{
	for(int i = 0; i < LINE_END ; ++i)
		Safe_Delete(m_pLine2D[i]);
}

bool Engine::CNavCell::IsPassed(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, NEIGHBOR* pNeighbor)
{
	for(int i = 0; i < NEIGHBOR_END; ++i)
	{
		if(CLine2D::DIR_LEFT == m_pLine2D[i]->Check_Dir(&D3DXVECTOR2(vPos.x + vDir.x, vPos.z + vDir.z)))
		{
			*pNeighbor = NEIGHBOR(i);
			
			return true;
		}
	}

	return false;
}

const Engine::CLine2D& Engine::CNavCell::GetLine(const LINE& eLine)
{
	return *(m_pLine2D[eLine]);
}

void Engine::CNavCell::SetPoint(D3DXVECTOR3* const pPointA)
{
	memcpy(m_vPoint, pPointA, sizeof(D3DXVECTOR3) * 3);


	for(int i = 0; i < LINE_END ; ++i)
		Safe_Delete(m_pLine2D[i]);

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);
}