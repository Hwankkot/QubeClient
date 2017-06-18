#include "stdafx.h"
#include "JohnStateObserver.h"

#include "Engine_Function.h"
#include "JohnStateSubject.h"

CJohnStateObserver::CJohnStateObserver(void)
: m_pJohnStateSubject(NULL)
{

}

CJohnStateObserver::~CJohnStateObserver(void)
{
	Release();
}

CJohnStateObserver* CJohnStateObserver::Create(CJohnStateSubject* pJohnStateSubject)
{
	CJohnStateObserver* pMouseObserver = new CJohnStateObserver;

	if(FAILED(pMouseObserver->InitJohnStateObserver(pJohnStateSubject)))
		Engine::Safe_Delete(pMouseObserver);

	return pMouseObserver;
}

HRESULT CJohnStateObserver::InitJohnStateObserver(CJohnStateSubject* pJohnStateSubject)
{
	m_pJohnStateSubject = pJohnStateSubject;

	ZeroMemory(m_IsDataChanged, sizeof(bool) * CJohnStateSubject::DATAMESSAGE_END);
	ZeroMemory(m_pData, sizeof(void*) * CJohnStateSubject::DATAMESSAGE_END);
	
	return S_OK;
}

void CJohnStateObserver::Update(const DWORD& dwMessage)
{
	m_IsDataChanged[dwMessage] = true;
}

void CJohnStateObserver::GetData(const DWORD& dwMessage, void* const pDataStorage
							, const DWORD& dwDataType, const DWORD& dwDataSize)
{
	/// ������ ��������� ��������� �����Ҵ����� ���� ������ �����.
	if(m_pData[dwMessage] == NULL)
	{
		DWORD dwSize = 0;

		switch(dwDataType)
		{
		case CJohnStateSubject::DATATYPE_FLOAT:
			dwSize = DWORD(dwDataSize / sizeof(float));
			m_pData[dwMessage] = new float[dwSize];
			ZeroMemory(m_pData[dwMessage], dwDataSize);
			break;

		default:
			NULL_CHECK_RETURN_MSG(m_pData[dwMessage], , L"The datatype which user inputed is wrong.");
			break;
		}
	}

	/// ���� �����Ͱ� ������ �Ǿ������� true���� �̹Ƿ� �����͸� �ٽ� �޾ƿ´�.
	if(m_IsDataChanged[dwMessage] == true)
	{
		m_pJohnStateSubject->GetData(dwMessage, m_pData[dwMessage], dwDataSize);
		m_IsDataChanged[dwMessage] = false;
	}

	memcpy(pDataStorage, m_pData[dwMessage], dwDataSize);
}

bool CJohnStateObserver::GetCheckDataChanged(const DWORD& dwMessage)
{
	return m_IsDataChanged[dwMessage];
}

void CJohnStateObserver::Release(void)
{
	/// ������ ���� ���� ����
	for(DWORD i = 0; i < CJohnStateSubject::DATAMESSAGE_END; ++i)
		Engine::Safe_Delete(m_pData[i]);
}

