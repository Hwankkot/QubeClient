#include "Observer.h"

Engine::CObserver::CObserver(void)
: m_dwIndex(-1)
{

}

Engine::CObserver::~CObserver(void)
{

}

void Engine::CObserver::SetIndex(const DWORD& dwIndex)
{
	m_dwIndex = dwIndex;
}

DWORD Engine::CObserver::GetIndex(void)
{
	return m_dwIndex;
}