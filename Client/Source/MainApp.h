/*!
 * \file MainApp.h
 * \date 2016/04/10 14:42
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
#ifndef MainApp_h__
#define MainApp_h__

namespace Engine
{
	class CFontMgr;
	class CGraphicDev;
	class CInput;
	class CManagement;
	class CRenderer;
	class CTimeMgr;
}

class CMainApp
{
private:
	explicit CMainApp(void);
public:
	~CMainApp(void);

// �⺻ �޼ҵ�
public:
	static CMainApp* Create(void);
	HRESULT InitMainApp(void);
	void Update(void);
	void Render(void);

private:
	void Release(void);

// ����̽� ����
private:
	LPDIRECT3DDEVICE9  m_pDevice; // 3D com ��ü ������

private:
	Engine::CInput*			m_pInput;
	Engine::CManagement*	m_pManagement;
	Engine::CRenderer*		m_pRenderer;
	Engine::CTimeMgr*		m_pTimeMgr;
};

#endif // MainApp_h__