/*!
 * \file Camera.h
 * \date 2016/04/10 15:46
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
#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera
	: public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera&);
	CCamera& operator = (const CCamera&);

public:
	virtual ~CCamera(void);

public:
	/// Call Invalidate_Viewmatrix.
	virtual void Update(void);

public:
	/// Renew View Matrix.
	/// Set LookAtLH, SetTransform by View Matrix.
	void Invalidate_ViewMatrix(void);

	/// Set Projection Matrix.
	/// Set PerspectiveFovLh, Set Transform by Projection Matrix.
	void Invalidate_ProjMatrix(void);

protected:
	D3DXMATRIX		m_matView;
	D3DXVECTOR3		m_vEye, m_vAt, m_vUp;

protected:
	D3DXMATRIX		m_matProj;
	float			m_fFovY, m_fAspect, m_fNear, m_fFar;
};

END

#endif // Camera_h__
