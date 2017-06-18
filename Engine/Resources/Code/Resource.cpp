#include "Resource.h"

Engine::CResource::CResource(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_dwRefCnt(1)
{

}

Engine::CResource::CResource(const CResource& rhs)
: m_pDevice(rhs.m_pDevice)
, m_dwRefCnt(rhs.m_dwRefCnt)
{

}

Engine::CResource::~CResource(void)
{

}

void Engine::CResource::AddRefCnt(void)
{
	++m_dwRefCnt;
}
