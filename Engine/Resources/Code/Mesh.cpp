#include "Mesh.h"

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pDevice)
: CResource(pDevice)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
: CResource(rhs.m_pDevice)
{

}

Engine::CMesh::~CMesh(void)
{

}

void Engine::CMesh::GetMinMax(STANDARD_VERTEX3* const StdVtx3)
{
	*StdVtx3 = m_StdVtx3;
}
