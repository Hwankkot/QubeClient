/*!
 * \file Transform.h
 * \date 2016/04/10 15:44
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

#ifndef Transform_h__
#define Transform_h__

#include "Component.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit CTransform( const D3DXVECTOR3& vLook );

public:
	virtual ~CTransform(void);

public:
	static CTransform* Create(const D3DXVECTOR3& vLook);

public:
	virtual void Update(void);
	virtual DWORD Release(void);

public:
	float				m_fAngle[ANGLE_END];
	D3DXVECTOR3			m_vScale;
	D3DXVECTOR3			m_vPos;
	D3DXVECTOR3			m_vDir;
	D3DXMATRIX			m_matWorld;
};

END

#endif // Transform_h__