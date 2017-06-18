/*!
 * \file StaticMesh.h
 * \date 2016/03/31 15:30
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

#ifndef StaticMesh_h__
#define StaticMesh_h__

/// Resources
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaticMesh(const CStaticMesh& rhs);

public:
	virtual	~CStaticMesh(void);

public:
	/// 스태틱 메시를 생성한다.
	/// 스태틱 메시가 잇는 파일의 경로
	/// 스태틱 메시 파일의 이름
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName);

public:
	/// 클론을 생성한다.
	virtual CResource* CloneResource(void);

public:
	/// X파일에서 Mesh를 불러온다.
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);

	/// Render Mesh file on world coordinate system.
	virtual void RenderMesh(const D3DXMATRIX* pWorldMatrix);

private:
	void MakeBoundingBox(void);

public:
	LPD3DXMESH GetMesh(void);

public:
	virtual DWORD Release(void);

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	D3DMATERIAL9*			m_pMtrl;

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pSubset;
	DWORD					m_dwSubsetCnt;
	D3DMATERIAL9			m_Material;
};

END

#endif // StaticMesh_h__