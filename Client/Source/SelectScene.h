/*!
 * \file SelectScene.h
 * \date 2016/04/10 14:59
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
#ifndef SelectScene_h__
#define SelectScene_h__

#include "Engine_Macro.h"
#include "Logo.h"
#include "Stage.h"

class CSelectScene
{
public:
	enum SCENE { SCENE_LOGO, SCENE_STAGE, SCENE_END };

public:
	explicit CSelectScene(SCENE eScene) : m_eScene(eScene) {}
	~CSelectScene(void) {}

public:
	HRESULT operator ()(Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice)
	{
		switch(m_eScene)
		{
		case SCENE_LOGO:
			*ppScene = CLogo::Create(pDevice);
			NULL_CHECK_RETURN(*ppScene, E_FAIL);
			break;

		case SCENE_STAGE:
			*ppScene = CStage::Create(pDevice);
			NULL_CHECK_RETURN(*ppScene, E_FAIL);
			break;

		default:
			__asm int 3;
			break;
		}

		return S_OK;
	}

private:
	SCENE  m_eScene;
};

#endif // SelectScene_h__