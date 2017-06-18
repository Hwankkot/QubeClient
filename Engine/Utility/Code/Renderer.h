
/*!
 * \file Renderer.h
 * \date 2016/04/10 15:35
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
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Enum.h"

BEGIN(Engine)

class CScene;
class CGameObject;
class CInput;
class ENGINE_DLL CRenderer
{
	DECLARE_SINGLETON(CRenderer)

public:
	enum RENDERID { RENDER_PRIORITY, RENDER_CUBE, RENDER_UI, RENDER_END };

private:
	explicit CRenderer(void);
	~CRenderer(void);

// Render ����
public:
	void Render(const float& fTime);

private:
	void Render_Priority(void);
	void Render_Cube(void);
	void Render_UI(void);

// �޼��� ��� ����
public:
	void SetMessage(const TCHAR* pMessage);

private:
	void RenderMessage(void);
	void Render_FPS(const float& fTime);

public:
	void AddRenderObject(RENDERID eRenderID, CGameObject* pGameObject);
	void DeleteRenderObject(RENDERID eRenderID, CGameObject* pGameObject);
	void ChangeCurrentScene(CScene* pScene);

public:
	void ResetRenderlist(RENDERID eRenderID);

public:
	std::vector<CGameObject*>& GetGameObjectlist(RENDERID eRenderID);

private:
	void Release(void);

private:
	CScene*		m_pScene;

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	
// FPS ��� ����
private:
	TCHAR	m_szFps[128];	// ��� �޼���
	DWORD	m_dwCount;		// ���� �ð� (����)
	float	m_fTime;		// ���� �ð� (�Ǽ�)

// �޼��� ��� ����
private:
	const TCHAR*	m_pMessage;
	TCHAR	m_szKeyState[256];

private:
	typedef std::vector<CGameObject*>	RENDERLIST;
	RENDERLIST	m_Renderlist[RENDER_END];
};

END

#endif // Renderer_h__