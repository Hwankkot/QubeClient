/*!
 * \file Stage.h
 * \date 2016/04/10 15:02
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
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

namespace Engine
{ 
	class CRenderer;
}

class CInputUnit;
class CStage : public Engine::CScene
{
public:
	enum LAYER { LAYER_UI, LAYER_QUBE, LAYER_ENVINRONMENT, LAYER_END };

private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CStage(const CStage&);
	CStage& operator = (const CStage&);
	
public:
	~CStage(void);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);	// 按眉 积己

// 檬扁拳 包访
private:
	HRESULT InitScene(void);				// 檬扁拳
	HRESULT AddUI(void);					// UI 积己
	HRESULT	AddEnvironment(void);			// Environment 积己
	HRESULT AddLight(void);					// Light 积己
	

private:
	virtual void Update(void);

public:
	virtual void AddLayer(const DWORD& dwContainerKey, Engine::CLayer* pLayer, const DWORD& dwRenderID);

private:
	Engine::CRenderer* m_pRenderer;

};

#endif // Stage_h__