#include "Layer.h"
#include "GameObject.h"

#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "GameObject.h"

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CLayer::~CLayer(void)
{
	Release();
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

void Engine::CLayer::Update(void)
{
	OBJECTLISTMAP::iterator	mapiter		= m_mapObjectlist.begin();
	OBJECTLISTMAP::iterator	mapiter_end = m_mapObjectlist.end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		size_t iSize = mapiter->second.size();

		OBJECTLIST& Objectlist = mapiter->second;

		for(size_t i = 0; i < iSize; ++i)
		{
			Objectlist[i]->Update();
		}
	}	
}

void Engine::CLayer::Render(void)
{
	OBJECTLISTMAP::iterator	mapiter		= m_mapObjectlist.begin();
	OBJECTLISTMAP::iterator	mapiter_end = m_mapObjectlist.end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		OBJECTLIST::iterator	iterlist	 = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for (; iterlist != iterlist_end; ++iterlist)		
		{
			(*iterlist)->Render();
		}		
	}	
}

void Engine::CLayer::Release(void)
{
	OBJECTLISTMAP::iterator	mapiter		= m_mapObjectlist.begin();
	OBJECTLISTMAP::iterator	mapiter_end = m_mapObjectlist.end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		OBJECTLIST::iterator	iterlist	 = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for (; iterlist != iterlist_end; ++iterlist)		
			Engine::Safe_Delete(*iterlist);

		mapiter->second.clear();
	}
	m_mapObjectlist.clear();
}


void Engine::CLayer::AddObject(const DWORD& dwObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		if(NULL == m_mapObjectlist.count(dwObjectKey))
		{ 
			m_mapObjectlist[dwObjectKey] = OBJECTLIST();
			m_mapObjectlist[dwObjectKey].reserve(100);
		}
		m_mapObjectlist[dwObjectKey].push_back(pGameObject);
	}
	return ;
}

void Engine::CLayer::RemoveObject(const DWORD& dwObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		if(NULL == m_mapObjectlist.count(dwObjectKey))
			NULL_CHECK_RETURN_MSG(NULL, , L"The object storage doesn't exist.");

		OBJECTLIST::iterator	iterlist	 = m_mapObjectlist[dwObjectKey].begin();
		OBJECTLIST::iterator	iterlist_end = m_mapObjectlist[dwObjectKey].end();
		for(; iterlist != iterlist_end;)
		{
			if( (*iterlist) == pGameObject )
			{
				if(iterlist == iterlist_end)
				{
					Safe_Delete(*iterlist);
				}

				else 
				{
					Safe_Delete(*iterlist);
					(*iterlist) = (*(--iterlist_end));
				}

				m_mapObjectlist[dwObjectKey].pop_back();
				break;
			}
			else
				++iterlist;
		}
	}
	return ;
}

void Engine::CLayer::SetMapObjectlist(std::map<DWORD, std::vector<Engine::CGameObject*>>* pmapObjectlist)
{
	m_mapObjectlist = (*pmapObjectlist);
}

std::map<DWORD, std::vector<Engine::CGameObject*>>* Engine::CLayer::GetMapObjectlist(void)
{
	return &m_mapObjectlist;
}