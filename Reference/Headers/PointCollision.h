/*!
 * \file PointCollision.h
 * \date 2016/04/19 11:00
 *
 * \author Architect
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef PointCollision_h__
#define PointCollision_h__

#include "Collision.h"

BEGIN(Engine)

class ENGINE_DLL CPointCollision : public CComponent
{
private:
	CPointCollision(void);

public:
	~CPointCollision(void);

	static CPointCollision* Create(void);


public:
	virtual DWORD Release(void);
};

END


#endif // PointCollision_h__