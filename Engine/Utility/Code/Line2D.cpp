#include "Line2D.h"

#include "Engine_Function.h"

Engine::CLine2D::CLine2D(void)
{

}

Engine::CLine2D::~CLine2D(void)
{

}

Engine::CLine2D* Engine::CLine2D::Create(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint)
{
	CLine2D* pLine2D = new CLine2D;

	if(FAILED(pLine2D->InitLine2D(pStartPoint, pEndPoint)))
		Safe_Delete(pLine2D);

	return pLine2D;
}

HRESULT Engine::CLine2D::InitLine2D(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint)
{
	// 3D에서의 벡터를 받아서 D3DXVECTOR2의 형태로 변경한다.
	// 그래야지만 2D 에서의 삼각형 법선 벡터를 구할 수 있다.
	m_vStartPoint = D3DXVECTOR2(pStartPoint->x, pStartPoint->z);
	m_vEndPoint = D3DXVECTOR2(pEndPoint->x, pEndPoint->z);

	// 변을 이루는 벡터를 구한다.
	D3DXVECTOR2	vDir = m_vEndPoint - m_vStartPoint;

	// 구한 벡터를 가지고 법선 벡터를 구한다.
	// 법선 벡터를 구하는 공식은 간단하다.
	// 법선 벡터는 기존의 벡터와 90도를 이루는 벡터이다.
	// 그렇기 때문에 내적의 결과값이 0으로 나오도록 하면 된다.
	// 임의의 벡터 V의 결과값이 0으로 나오기 위해서는 V1 = V2는 같은 벡터.
	// 0 = (V1.x * (V2.y * -1)) + (V1.y * (V2.x)

	m_vNormal = D3DXVECTOR2(vDir.y * -1, vDir.x);
	
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

Engine::CLine2D::DIR Engine::CLine2D::Check_Dir(const D3DXVECTOR2* pMovedPoint)
{
	D3DXVECTOR2 vDir = *pMovedPoint - m_vStartPoint;

	float	fDotResult = D3DXVec2Dot(&vDir, &m_vNormal);

	if(fDotResult > 0)
		return DIR_LEFT;
	else
		return DIR_RIGHT;
}

const D3DXVECTOR2& Engine::CLine2D::GetNormalVector(void) const
{
	return m_vNormal;
}

