/*!
 * \file Mesh.h
 * \date 2016/04/18 0:31
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

#ifndef Mesh_h__
#define Mesh_h__

/// Resources
#include "Resource.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class CVIBuffer;
class ENGINE_DLL CMesh : public CResource
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& rhs);

public:
	virtual ~CMesh(void);

public:
	virtual CResource* CloneResource(void) PURE;

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName) PURE;
	virtual void RenderMesh(const D3DXMATRIX* pWorldMatrix) PURE;

public:
	void GetMinMax(STANDARD_VERTEX3* const StdVtx3);
	void SetBoundingBoxColor(DWORD dwColor);

protected:
	STANDARD_VERTEX3 m_StdVtx3;
	CVIBuffer*	m_pBoundingBox;
};

END

#endif // Mesh_h__