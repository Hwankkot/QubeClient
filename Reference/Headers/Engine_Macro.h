#ifndef Engine_Macro_h__
#define Engine_Macro_h__

#define MSG_BOX(_message)	MessageBox(NULL, _message, L"System Message", MB_OK)
#define TAGMSG_BOX(_tag,_message)	MessageBox(NULL, _message, _tag, MB_OK)

#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}

#define NULL_CHECK_RETURN_TAGMSG( _ptr, _return, _tag, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, _tag, MB_OK);__asm { int 3 };return _return;}}\




#define EXIST_CHECK( _ptr)	\
{if( _ptr != NULL){__asm { int 3 };return;}}

#define EXIST_CHECK_RETURN( _ptr, _return)	\
{if( _ptr != NULL){__asm { int 3 };return _return;}}

#define EXIST_CHECK_MSG( _ptr, _message )		\
{if( _ptr != NULL){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define EXIST_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr != NULL){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}

#define EXIST_CHECK_RETURN_TAGMSG( _ptr, _return, _tag, _message )	\
{if( _ptr != NULL){MessageBox(NULL, _message, _tag, MB_OK);__asm { int 3 };return _return;}}



#define FAILED_CHECK( _hr)														\
	if( ((HRESULT)(_hr)) < 0 )													\
{																				\
	char szFilePath[MAX_PATH];													\
	char szMessage[MAX_PATH];													\
	char szExt[MAX_PATH];														\
	strcpy_s(szFilePath, MAX_PATH, __FILE__);								   	\
	_splitpath_s(szFilePath, 0, 0, 0, 0, szMessage, MAX_PATH, szExt, MAX_PATH);	\
	strcat_s(szMessage, MAX_PATH, szExt);										\
	DWORD dwLine = __LINE__;													\
	char szLineMessage[MAX_PATH];												\
	sprintf_s(szLineMessage, MAX_PATH, "\nLine: %d \n", dwLine);				\
	strcat_s(szMessage, MAX_PATH, szLineMessage);								\
	MessageBoxA(NULL, szMessage, "System Error",MB_OK);							\
	__asm { int 3 };															\
	return E_FAIL;																\
}															

#define FAILED_CHECK_RETURN(_hr, _return)										\
	if( ((HRESULT)(_hr)) < 0 )													\
{																				\
	char szFilePath[MAX_PATH];													\
	char szMessage[MAX_PATH];													\
	char szExt[MAX_PATH];														\
	strcpy_s(szFilePath, MAX_PATH, __FILE__);								   	\
	_splitpath_s(szFilePath, 0, 0, 0, 0, szMessage, MAX_PATH, szExt, MAX_PATH);	\
	strcat_s(szMessage, MAX_PATH, szExt);										\
	DWORD dwLine = __LINE__;													\
	char szLineMessage[MAX_PATH];												\
	sprintf_s(szLineMessage, MAX_PATH, "\nLine: %d \n", dwLine);				\
	strcat_s(szMessage, MAX_PATH, szLineMessage);								\
	MessageBoxA(NULL, szMessage, "System Error",MB_OK);							\
	__asm { int 3 };															\
	return _return;																\
}

#define FAILED_CHECK_MSG( _hr, _message)															\
if( ((HRESULT)( _hr)) < 0 )																			\
{																									\
	TCHAR szTotalMessage[512];																		\
	lstrcpy( szTotalMessage, _message);																\
	lstrcat( szTotalMessage, L"\n");																\
	char szFilePath[MAX_PATH];																		\
	char szMessage[256];																			\
	char szExt[128];																				\
	strcpy_s(szFilePath, MAX_PATH, __FILE__);								   						\
	_splitpath_s(szFilePath, 0, 0, 0, 0, szMessage, 256, szExt, 128);								\
	strcat_s(szMessage, 128, szExt);																\
	DWORD dwLine = __LINE__;																		\
	char szLineMessage[MAX_PATH];																	\
	sprintf_s(szLineMessage, MAX_PATH, "\nLine: %d \n", dwLine);									\
	strcat_s(szMessage, 256, szLineMessage);														\
	TCHAR szConvertedMessage[512];																	\
	int iLength = MultiByteToWideChar(CP_ACP, 0, szMessage, strlen(szMessage), NULL, NULL) + 1;		\
	MultiByteToWideChar(CP_ACP, 0, szMessage, -1, szConvertedMessage, iLength);						\
	lstrcat( szTotalMessage, szConvertedMessage);													\
	MessageBox(NULL, szTotalMessage, L"System Error", MB_OK);										\
	return E_FAIL;																					\
}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)											\
	if( ((HRESULT)( _hr)) < 0 )																		\
{																									\
	TCHAR szTotalMessage[512];																		\
	lstrcpy( szTotalMessage, _message);																\
	lstrcat( szTotalMessage, L"\n");																\
	char szFilePath[MAX_PATH];																		\
	char szMessage[256];																			\
	char szExt[128];																				\
	strcpy_s(szFilePath, MAX_PATH, __FILE__);								   						\
	_splitpath_s(szFilePath, 0, 0, 0, 0, szMessage, 256, szExt, 128);								\
	strcat_s(szMessage, 128, szExt);																\
	DWORD dwLine = __LINE__;																		\
	char szLineMessage[MAX_PATH];																	\
	sprintf_s(szLineMessage, MAX_PATH, "\nLine: %d \n", dwLine);									\
	strcat_s(szMessage, 256, szLineMessage);														\
	TCHAR szConvertedMessage[512];																	\
	int iLength = MultiByteToWideChar(CP_ACP, 0, szMessage, strlen(szMessage), NULL, NULL) + 1;		\
	MultiByteToWideChar(CP_ACP, 0, szMessage, -1, szConvertedMessage, iLength);						\
	lstrcat( szTotalMessage, szConvertedMessage);													\
	MessageBox(NULL, szTotalMessage, L"System Error", MB_OK);										\
	return _return;																					\
}

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }


#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport) 
#else
#define ENGINE_DLL _declspec(dllimport) 
#endif


#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
		if(NULL != m_pInstance)	{					\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}


#endif // Engine_Macro_h__
