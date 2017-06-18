#include "Subject.h"

#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Observer.h"

Engine::CSubject::CSubject(void)
{

}

Engine::CSubject::~CSubject(void)
{
	Release();
}

void Engine::CSubject::Subscribe(CObserver* pObserver)
{
	/// 옵저버 포인터가 있는지 확인
	NULL_CHECK_MSG(pObserver, L"The Observer doesn't exist");

	/// 인덱스는 옵저버의 저장 위치를 찾기 위해서 쓰인다.
	/// 각 옵저버의 인덱스는 추가되기 전의 현재 사이즈로 자동 입력된다.
	pObserver->SetIndex(m_ObserversList.size());

	/// 옵저버를 추가한다.
	m_ObserversList.push_back(pObserver);
}

void Engine::CSubject::Unsubscribe(const DWORD& dwIndex, const CObserver* pObserver)
{
	/// 현재 인덱스에 존재하는 옵저버가 입력받은 옵저버와 같은지 확인
	if(m_ObserversList[dwIndex] != pObserver)
		FAILED_CHECK_RETURN_MSG(E_FAIL, , L"The index is not matched");

	/// 마지막 옵저버의 위치를 받음
	DWORD dwLastIndex = m_ObserversList.size() - 1;

	/// 삭제하려는 위치에 마지막 옵저버를 넣음
	m_ObserversList[dwIndex] = m_ObserversList[dwLastIndex];
	
	/// 인덱스 갱신
	m_ObserversList[dwIndex]->SetIndex(dwIndex);

	/// 마지막 옵저버가 있던 저장소를 삭제
	m_ObserversList.pop_back();
}

void Engine::CSubject::Notify(const DWORD& dwMessage)
{
	/// Push 옵저버를 형성하는 프로그램 구문
	/// 만약 Pull 옵저버로 변경하고 싶으면 상속을 받아서 코드를 변경하면 된다.
	/// 아니면 Observer가 갱신될 때 어떤 값이 변경되었는 지만 체크를 해서
	/// 변경된 값이 있을 경우 Subject에서 값을 가져오도록 코딩할 수 있다.
	OBSERVERLIST::iterator	iterlist  = m_ObserversList.begin();
	OBSERVERLIST::iterator	iterlist_end = m_ObserversList.end();

	for(; iterlist != iterlist_end; ++iterlist)
	{
		(*iterlist)->Update(dwMessage);
	}
}


void Engine::CSubject::Release(void)
{
	m_ObserversList.clear();
	m_ObserversList.swap(OBSERVERLIST());
}

