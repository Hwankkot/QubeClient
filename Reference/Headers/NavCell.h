/*!
 * \file NavCell.h
 * \date 2016/04/07 18:06
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
#ifndef NavCell_h__
#define NavCell_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CLine2D;

class ENGINE_DLL CNavCell
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };

private:
	CNavCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);

public:
	~CNavCell(void);

public:
	static CNavCell* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC
		, const DWORD& dwIdx);
	
	HRESULT InitNavCell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx);
	
public:
	const D3DXVECTOR3* GetPoint(POINT ePointID);
	void SetPoint(D3DXVECTOR3* const pPointA);
public:
	DWORD GetIndex(void);

public:
	void SetNeighbor(NEIGHBOR eNeighberID, CNavCell* pNeighbor);
	CNavCell* GetNeighbor(NEIGHBOR eNeighborID);

public:
	const CLine2D& GetLine(const LINE& eLine);

public:
	bool Compare_Point(const D3DXVECTOR3* pPointStart, const D3DXVECTOR3* pPointEnd, CNavCell* pNeighbor);
	void Render(LPD3DXLINE pLine);
	bool IsPassed(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, NEIGHBOR* pNeighbor);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9 m_pDevice;

private:
	CLine2D*		m_pLine2D[LINE_END];
	CNavCell*		m_pNeighbor[NEIGHBOR_END];
	D3DXVECTOR3		m_vPoint[POINT_END];
	DWORD m_dwIndex;
};

END

#endif // NavCell_h__