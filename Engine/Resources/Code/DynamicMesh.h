/*!
 * \file DynamicMesh.h
 * \date 2016/04/06 16:47
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

#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class CHierarchyLoader;
class CAniCtrl;

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CDynamicMesh(const CDynamicMesh& rhs);

public:
	virtual ~CDynamicMesh(void);

public:
	DOUBLE GetTrackPosition(void);
	DOUBLE GetPeriod(void);
	const D3DXMATRIX* FindFrame(const char* pFrameName);

public:
	void SetAnimationSet(UINT iAniIdx, DOUBLE dAccTime);
	void SetTrackPos(DOUBLE dTrackPos);
	void FrameMove(const float& fTimePerSec);

public:
	virtual CResource* CloneResource(void);

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void RenderMesh(const D3DXMATRIX* pWorldMatrix);
	virtual DWORD Release(void);

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath
		, const TCHAR* pFileName);

private:
	void UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	void SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame);
	void Find_MeshContainer(DERIVED_FRAME* pFrame);
	void Render_MeshContainer(DERIVED_MESHCONTAINER* pMeshContainer);
	void MakeBoundingBox(D3DXFRAME* pFrame, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);

private:
	D3DMATERIAL9			m_Material;
	LPD3DXFRAME				m_pRootBone;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
};

END

#endif // DynamicMesh_h__