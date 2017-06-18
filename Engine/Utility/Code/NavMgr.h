/*!
 * \file NavMgr.h
 * \date 2016/04/07 19:35
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

// 기본 Progress
// NavMgr를 만들고 Export_Utility에 inline 함수 추가.
// Stage에서 Add_NavCell 함수를 만들고
// Cell을 추가한다.
// Player를 추가적으로 만들고 Player 내에서 NavCell을 적용시킨다.

#ifndef NavMgr_h__
#define NavMgr_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CNavCell;

class ENGINE_DLL CNavMgr
{
	DECLARE_SINGLETON(CNavMgr);

private:
	CNavMgr(void);
	~CNavMgr(void);

public:
	// Vector 안의 NavCell 삭제
	void Release(void);

public:
	// STL벡터의 크기를 dwSize 만큼 미리 할당하고 Line com 객체를 만든다.
	HRESULT InitNavMeshManager(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwContainerSize);
	// 인자값으로 받은 점들을 가지고 CNavCell을 생성한다.
	HRESULT AddCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);
	// 모든 셀들의 이웃을 설정한다.
	void LinkCell(void);

	DWORD MoveOnNavMesh(D3DXVECTOR3* pEye, D3DXVECTOR3* pAt, D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx);
	
	// 생성한 모든 NavCell들을 렌더링한다.
	void RenderNavMesh(void);

	std::vector<CNavCell*>* GetNavMesh(void);



private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXLINE			m_pLine;
	DWORD				m_dwReservedSize;
	DWORD				m_dwIdxCnt;

private:
	typedef std::vector<CNavCell*>  NAVMESH;
	NAVMESH  m_NavMesh;
};

END

#endif // NavMgr_h__