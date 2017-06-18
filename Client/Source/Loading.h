/*!
 * \file Loading.h
 * \date 2016/04/16 11:09
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
#ifndef Loading_h__
#define Loading_h__

namespace Engine{ class CLayer; }

class CLoading
{
private:
	CLoading(void);
	
	/// 복사 생성자 및 대입연산자로 생성을 막음
	explicit CLoading(const CLoading&);
	CLoading& operator = (const CLoading&);

public:
	~CLoading(void);

public:
	static CLoading* Create(void);
	HRESULT InitLoading(void);
private:
	void Release(void);

public:
	static UINT WINAPI LoadingFunction(void* pArg);

private:
	void LoadResources(void);

public:
	const TCHAR* GetLoadingMessage(void);
	bool GetComplete(void);

public:
	Engine::CLayer* GetCubeLayer(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR					m_szLoadingMessage[128];
	bool					m_bComplete;

private:
	Engine::CLayer* m_pCubeLayer;
};
#endif // Loading_h__