/*!
 * \file CubeColor.h
 * \date 2016/04/17 23:51
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
#ifndef CubeColor_h__
#define CubeColor_h__

#include "VIBuffer.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor
	: public CVIBuffer
{
private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubeColor(void);

public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 pDevice, const STANDARD_VERTEX3& StdVtx3, const DWORD& dwColor);
	
	virtual CResource* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(const STANDARD_VERTEX3& StdVtx3, const DWORD& dwColor);
	
};

END

#endif // CubeColor_h__
