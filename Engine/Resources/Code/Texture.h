/*!
 * \file Texture.h
 * \date 2015/04/06 21:48
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
#ifndef Texture_h__
#define Texture_h__

#include "Engine_Enum.h"
#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CTexture
	: public CResource
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& rhs);

public:
	virtual ~CTexture(void);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice
		, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	virtual DWORD Release(void);
	virtual CResource* CloneResource(void);

public:
	/// Load Texture.
	HRESULT LoadTexture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	void Render(const DWORD& dwStage, const DWORD& iIndex);

private:
	typedef std::vector<IDirect3DBaseTexture9*>  VECTEXTURE;
	VECTEXTURE  m_vecTexture;

private:
	DWORD  m_dwContainerSize;
};

END

#endif // Texture_h__