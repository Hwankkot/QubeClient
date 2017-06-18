#include "stdafx.h"
#include "Scenery.h"

#include "Engine_Function.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Texture.h"
#include "Transform.h"
#include "Value.h"
#include "VIBuffer.h"

CScenery::CScenery(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
{
	
}

CScenery::~CScenery(void)
{

}

CScenery* CScenery::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScenery* pGameObject = new CScenery(pDevice);
	
	if(FAILED(pGameObject->InitScenery()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

HRESULT CScenery::InitScenery(void)
{
	FAILED_CHECK(AddComponent());

	m_pState->m_vScale = D3DXVECTOR3(2.f, 2.f, 2.f);

	return S_OK;
}

void CScenery::Update(void)
{
	Engine::CGameObject::Update();
}

void CScenery::Render(void)
{
	m_pTexture->Render(0,0);
	m_pBuffer->Render(&m_pState->m_matWorld);
}

HRESULT CScenery::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pState = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TRANSFORM, pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::TEXBUFFER_RECTANGLE);
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TEXBUFFER_RECTANGLE, pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::RECTTEXTURE_LOGO);
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(TEXTURE_RECTANGLE, pComponent));

	return S_OK;
}



