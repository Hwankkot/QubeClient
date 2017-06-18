/*!
 * \file Engine_Struct.h
 * \date 2015/04/04 23:03
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
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture;
		LPD3DXMESH					pOriMesh;
		D3DXMATRIX*					pFrameOffsetMatrix;
		D3DXMATRIX**				ppFrameConbinedMatrix;
		D3DXMATRIX*					pResultMatrix;
	}DERIVED_MESHCONTAINER;

	typedef struct tagFrame_Derived : public D3DXFRAME
	{
		D3DXMATRIX		CombindedMatrix;
	}DERIVED_FRAME;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vNormal;
		D3DXVECTOR2				vTex;
	}VTXTEX;
	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexColor
	{
		D3DXVECTOR3				vPos;
		DWORD					dwColor;
	}VTXCOL;
	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	/// CubeTexture를 안 쓰게 되면 삭제할 것
	typedef struct tagCubeTex
	{
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vTex;
	}VTXCUBE;
	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;

	typedef struct tagRay3
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 dir;

		tagRay3() {}
		tagRay3(const D3DXVECTOR3& _pos3, const D3DXVECTOR3& _dir3)
			: pos(_pos3), dir(_dir3) {}
	}RAY3;

	typedef struct tagStandardVertex3
	{
		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
		
		tagStandardVertex3() {}
		tagStandardVertex3(const D3DXVECTOR3& _min3, const D3DXVECTOR3& _max3)
			: _min(_min3), _max(_max3) {}

	}STANDARD_VERTEX3;

	typedef struct tagCubeData
	{
		DWORD				dwCubeType;
		union {
			struct {
				float   fAngle_x;
				float   fAngle_y;
				float   fAngle_z;

			};
			float fAngle[3];
		};
		
		D3DXVECTOR3			vScale;
		D3DXVECTOR3			vPos;
		D3DXVECTOR3			vDir;
		D3DXMATRIX			matWorld;
	}CUBEDATA;
}

#endif // Engine_Struct_h__