#include "GameObject.h"

#include "Component.h"
#include "Engine_Function.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CGameObject::~CGameObject(void)
{
	Release();
}

void Engine::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for(; iter != iter_end ; ++iter)
	{
		iter->second->Update();
	}
}

void Engine::CGameObject::Compute_ViewZ(const D3DXVECTOR3& vPos)
{
	D3DXMATRIX	matViewState;

	/// View 행렬을 받아와서
	m_pDevice->GetTransform(D3DTS_VIEW, &matViewState);
	/// 역행렬로 만들면 카메라의 상태 행렬이 된다.
	D3DXMatrixInverse(&matViewState, NULL, &matViewState);

	/// 현재 카메라의 위치를 받아와서
	D3DXVECTOR3 vCamPos;
	memcpy(&vCamPos, &matViewState.m[3][0], sizeof(D3DXVECTOR3));
	
	/// Z값(현재 카메라와 물체의 위치까지의 거리)을 구한다.
	float fDistance = D3DXVec3Length( &(vCamPos - vPos) );

	m_fViewZ = fDistance;
}

const float& Engine::CGameObject::Get_ViewZ(void) const
{
	return m_fViewZ;
}

void Engine::CGameObject::Release(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	// FlyWeight와 Clone 객체를 동시에 지움
	
	DWORD		dwRefCnt = 0;
	for( ; iter != iter_end; ++iter)
	{
		dwRefCnt = iter->second->Release();
		if(dwRefCnt == 0)
			Safe_Delete(iter->second);
	}

	m_mapComponent.clear();
}

