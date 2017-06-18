/*!
 * \file Line2D.h
 * \date 2016/04/07 17:46
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
#ifndef Line2D_h__
#define Line2D_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CLine2D
{
public:
	enum DIR { DIR_LEFT, DIR_RIGHT };

private:
	CLine2D(void);
public:
	~CLine2D(void);

public:
	// 생성 시에 시작 점과 끝 점을 Parameter(인자값)로 받는다.
	// Create 안에서 InitLine2D를 호출해서 점 사이의 벡터와 법선벡터를 구한다.
	static CLine2D* Create(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);

public:
	// 점 사이의 벡터와 법선 벡터를 구해서 멤버 변수에 넣어준다.
	HRESULT InitLine2D(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);
	// \return 움직이고 난 후의 점을 받아서 벡터를 만든 후 법선벡터와 비교해서
	// 오른쪽에 있는지 왼쪽에 있는 지를 판단 후 리턴한다. 
	DIR Check_Dir(const D3DXVECTOR2* pMovedPoint);
	
	// NormalVector를 반환한다.
	const D3DXVECTOR2& GetNormalVector(void) const;

private:
	// 벡터의 시작점.
	D3DXVECTOR2 m_vStartPoint;
	// 벡터의 끝점.
	D3DXVECTOR2 m_vEndPoint;
	// 법선벡터.
	D3DXVECTOR2 m_vNormal;
};

END

#endif // Line2D_h__