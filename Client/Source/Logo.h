/*!
 * \file Logo.h
 * \date 2016/04/16 20:32
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

#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"

namespace Engine
{
	class CManagement;
	class CRenderer;
}

class CLoading;
class CLogo : public Engine::CScene
{
private:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_END };

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLogo(void);

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);		// ��ü ����

// �ʱ�ȭ ����
private:
	HRESULT InitLogo(void);				// �ʱ�ȭ
	HRESULT AddEnvironmentLayer(void);	// ������Ʈ ����

private:
	virtual void Update(void);
	void Release(void);

private:
	Engine::CRenderer*		m_pRenderer;
	CLoading*	m_pLoading;
};

#endif // Logo_h__
