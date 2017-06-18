#include "stdafx.h"
#include "Cube.h"

#include "StaticMesh.h"

const float CCube::CUBESCALE = 0.0625f;
const float CCube::CUBERADIUS = 2.f;

CCube::CCube(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInput(NULL)
, m_pLayer(NULL)
, m_pRenderer(NULL)
, m_pResourceMgr(NULL)
, m_pState(NULL)
, m_pStaticMesh(NULL)
, m_pSphere(NULL)
, m_IsIntersectedtoSelect(false)
, m_IsSelected(false)
, m_IsUpdate(true)
{
	
}

CCube::~CCube(void)
{

}

const Engine::CTransform* const CCube::GetCubeData(void) const
{
	return m_pState;
}

DWORD CCube::GetCubeType(void)
{
	return m_dwCubeType;
}

LPD3DXMESH CCube::GetMesh(void)
{
	return m_pStaticMesh->GetMesh();
}

void CCube::GetMinMax(Engine::STANDARD_VERTEX3* const pStdVtx3)
{
	if(m_pStaticMesh != NULL)
		m_pStaticMesh->GetMinMax(pStdVtx3);
}

void CCube::RunUpdate(void)
{
	m_IsUpdate = true;
}

void CCube::StopUpdate(void)
{
	m_IsUpdate = false;
}
