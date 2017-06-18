/*!
 * \file QuadTree.h
 * \date 2016/04/06 19:15
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
#ifndef QuadTree_h__
#define QuadTree_h__

/// Engine
#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CQuadTree
{
	DECLARE_SINGLETON(CQuadTree)

public:
	/// QuadTree를 구성할 Child의 Key Value.
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };

	/// QuadTree의 Vertex들을 나타내는 Index array의 Key value. 
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	
	/// 자신을 주변에 있는 QuadTree들을 저장하는 array의 키 값.
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	CQuadTree(void);

public:
	~CQuadTree(void);

/// QuadTree
public:
	/// QuadTree의 첫 번째 Node를 설정하고,  
	/// QuadTree를 구성하는 각 정점들의 Index값을 m_wCorner에 저장한다.
	HRESULT InitQuadTree(const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);

	/// Frustum 안에 있는지를 판별하기 위해서 
	/// Root Node의 QuadTree가 Frustum 안에 있는지를 SphereInFrustum(CFrustum 안의 함수)를
	/// 호출 해서 확인한다. 
	/// 함수의 인자값으로
	/// TerrainVtx를 입력받아서 Quadtree가 가지고 있는 Center의 점의 Index를 받아
	/// Terrain에서의 위치를 찾고, 반지름을 입력해준다. 
	/// 만약 Frustum 안에 Quadtree가 존재한다면 그 Quadtree의 멤머변수 m_isIn의 값을
	/// True로 변경한다.
	void IsinFrustum(const VTXTEX* pTerrainVtx);

	/// 인자 값이 true인 Quadtree들 안의 자식 Quadtree들을 조사해서 삼각형을 구성한다.
	/// 구성하는 삼각형들은 자식 노드가 없는 Child만 가능하다.
	void CullingToQuadTree(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);

private:
	/// Radius을 구하고 m_fRadius를 채워준다. Child Quadtree를 생성한다.
	void SetChild(const VTXTEX* pTerrainVtx);

	/// Child Quadtree를 생성한다.
	CQuadTree* MakeChild(CHILD eChildID);
	


/// Level of Detail
public:
	/// 주변의 이웃을 만든다. 
	HRESULT InitNeighbor(const VTXTEX* pTerrainVtx
		, CQuadTree* pParentQuad, const WORD& wCntX, const WORD& wCntZ);
	void Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);

private:
	/// 카메라의 위치와 각 삼각형의 중점과의 거리를 구해서
	/// 현재 함수를 불러온 Quadtree의 가로 길이보다 길면 자세하게 그리도록 조건을 만들어준다.
	/// Bool 값으로 그 길이를 비교해서 확인할 수 있다.
	bool LevelofDetail(const VTXTEX* pTerrainVtx);

	/// 존재하는 이웃들의 주소 값을 받아서 저장한다.
	bool MakeNeighbor(NEIGHBOR eNeighborID
		, int &iLT, int &iRT, int &iLB, int &iRB
		, const WORD& wCntX, const WORD& wCntZ);

	/// 내가 만든 Neighbor Quadtree의 찾아서 pointer를 반환한다.
	CQuadTree* FindNeighbor(const VTXTEX* pTerrainVtx
		, const int &iLT, const int &iRT, const int &iLB, const int &iRB
		, const int& iCenter);

private:
	void Release(void);

private:
	CQuadTree*		m_pChild[CHILD_END];
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	WORD			m_wCorner[CORNER_END];
	int				m_iCenter;
	float			m_fRadius;
	bool			m_isIn;
};

END


#endif // QuadTree_h__