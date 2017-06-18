#include "stdafx.h"
#include "JohnStateSubject.h"

#include "Engine_Function.h"
#include "Engine_Functor.h"

IMPLEMENT_SINGLETON(CJohnStateSubject)

CJohnStateSubject::CJohnStateSubject(void)
{

}

CJohnStateSubject::~CJohnStateSubject(void)
{
	Release();
}

void CJohnStateSubject::AddData(const DWORD& dwMessage, const DWORD& dwDataType
							, const DWORD& dwDataSize, const DWORD& dwContainerSize /* = 0 */)
{
	/// 인자 값을 메세지로 변환
	DATAMESSAGE eDataMessage = DATAMESSAGE(dwMessage);

	/// 키 값으로 데이터가 맵에 존재하는지 찾는다.
	DATAMAP::iterator mapiter = m_mapDataContainer.find(eDataMessage);

	/// 데이터가 존재하면 종료한다.
	if(mapiter != m_mapDataContainer.end())
	{
		MSG_BOX(L"The Data already exists");
		return ;
	}

	void* pData = NULL;
	DWORD dwSize = 0;

	switch(dwDataType)
	{
	case DATATYPE_FLOAT:
		dwSize = DWORD(dwDataSize / sizeof(float));
		pData = new float[dwSize];
		ZeroMemory(pData, dwDataSize);
		break;

	default:
		NULL_CHECK_RETURN_MSG(pData, , L"The datatype which user inputed is wrong.");
		break;
	}

	/// 추가하는 데이터를 참조할 옵저버들의 공간을 만듬
	if(dwContainerSize != 0)
		m_ObserversList.reserve(dwContainerSize);

	/// 참조할 데이터의 공간의 주소값을 받는다.
	m_mapDataContainer.insert(DATAMAP::value_type(eDataMessage, pData));
}

void CJohnStateSubject::GetData(const DWORD& dwMessage, void* pDataStorage, const DWORD& dwDataSize)
{
	/// 인자 값을 메세지로 변환
	DATAMESSAGE eDataMessage = DATAMESSAGE(dwMessage);

	/// 키 값으로 데이터가 맵에 존재하는지 찾는다.
	DATAMAP::iterator mapiter = m_mapDataContainer.find(eDataMessage);

	/// 데이터가 없으면 
	if(mapiter == m_mapDataContainer.end())
	{
		MSG_BOX(L"The Data doesn't exists");
		return ;
	}

	memcpy(pDataStorage, mapiter->second, dwDataSize);
}

void CJohnStateSubject::NotifyJohnState(const DWORD& dwMessage, void* pDataSource, const DWORD& dwDataSize)
{
	/// 데이터 저장공간이 비어있으면
	/// 참조할 데이터가 추가가 되어 있지 않음을 뜻한다. (AddData에서 설정한다.)
	if(m_mapDataContainer.find(dwMessage) == m_mapDataContainer.end())
	{
		MSG_BOX(L"The reference data doesn't exist");
		return ;
	}
	memcpy(m_mapDataContainer[dwMessage], pDataSource, dwDataSize);

	CSubject::Notify(dwMessage);
}

void CJohnStateSubject::Release(void)
{
	DATAMAP::iterator mapiter = m_mapDataContainer.begin();
	DATAMAP::iterator mapiter_end = m_mapDataContainer.end();

	for(; mapiter != mapiter_end; ++mapiter)
	{
		Engine::Safe_Delete(mapiter->second);
	}
}
