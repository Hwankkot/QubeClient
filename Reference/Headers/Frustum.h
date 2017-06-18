/*!
 * \file Frustum.h
 * \date 2016/03/30 16:33
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

/// TODO: change frustum to component from singleton


#ifndef Frustum_h__
#define Frustum_h__

/// Engine
#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum
{
	DECLARE_SINGLETON(CFrustum)

private:
	CFrustum(void);
	~CFrustum(void);

public:
	/// Frustum의 Vertex들을 설정하고 Projection Matrix의 역행렬로 연산해서
	/// View 좌표로 내리는 함수
	HRESULT InitFrustum(LPDIRECT3DDEVICE9 pDevice);

	/// View의 역행렬을 곱해서 월드 좌표로 내리는 함수
	void Transform_Frustum_ToWorld(void);

	/// 지형의 월드좌표를 이용해 Frustum의 좌표들을 로컬로 내린 후에 평면을 만든다.
	void Transform_Frustum_ToLocal(const D3DXMATRIX* pWorld);

	/// 로컬로 내린 절두체와 Terrain Vtx(로컬 좌표 상태인 정점들)의 위치를 비교해서
	/// 절두체 안의 점들을 검사한 후에 삼각형을 그려준다. 
	void CullingForTerrain(INDEX32* pIndex, DWORD* pTriCnt,
		const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);

	/// CullingforTerrain에서 사용하는 함수
	/// 검사할 Vertex Point를 인자로 받아서 현재 절두체의 면 6개와 비교 연산해서
	/// 정점이 출력될 수 있는지를 bool 형태의값으로 리턴.
	bool VertexInFrustum(const D3DXVECTOR3* pPoint);
	
	/// Quadtree가 Frustum 안에 있는지를 확인하기 위해 추가된 함수
	/// Quadtree를 작성하면서 추가되었다.
	bool SphereInFrustum(const D3DXVECTOR3* pCenter, const float& fRadius);

private:
	/// Local Space로 내린 Vertex들을 가지고 Plane을 생성한다.
	void MakePlane(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	D3DXVECTOR3			m_vPoint[8];
	D3DXVECTOR3			m_vConPoint[8];
	D3DXPLANE			m_Plane[6];
};

END

#endif // Frustum_h__