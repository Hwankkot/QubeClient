#include "StdAfx.h"
#include "SoundMgr.h"
#include "Functor.h"

CSoundMgr*	CSoundMgr::m_pInstance = NULL;

CSoundMgr::CSoundMgr(void)
{
	m_pSystem = NULL;
	m_iVersion = 0;

	Initialize();
}

CSoundMgr::~CSoundMgr(void)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter = m_mapSound.begin();
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter_end = m_mapSound.end();

	for(; iter != iter_end; ++iter)
	{
		m_Result = FMOD_Sound_Release(iter->second);
		delete [](iter->first);
	}
	m_mapSound.clear();

	m_Result = FMOD_System_Close(m_pSystem);
	m_Result = FMOD_System_Release(m_pSystem);
}

void CSoundMgr::Initialize(void)
{
	m_Result = FMOD_System_Create(&m_pSystem);

	m_Result = FMOD_System_GetVersion(m_pSystem, &m_iVersion);

	m_Result = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
}

void CSoundMgr::LoadSoundFile(void)
{
	_finddata_t		fd;
	long			handle;
	int				iResult = 1;

	handle = _findfirst("..\\..\\Data\\Resources\\Sound\\*.*", &fd);	//현재 폴더 내 모든 파일을 찾는다.

	if(handle == -1)
	{
		return;
	}

	while(iResult != -1)
	{
		TCHAR	szName[256];
		ZeroMemory(szName, sizeof(szName));

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fd.name, strlen(fd.name), szName, 256);
		TCHAR*	pName = new TCHAR[256];
		ZeroMemory(pName, sizeof(TCHAR) * 256);
		lstrcpy(pName, szName);

		char szFullName[256];
		strcpy_s(szFullName, "..\\..\\Data\\Resources\\Sound\\");
		strcat_s(szFullName, fd.name);

		FMOD_SOUND*		pSound;
		m_Result = FMOD_System_CreateSound(m_pSystem, szFullName, FMOD_HARDWARE, 0, &pSound);

		if(m_Result == FMOD_OK)
			m_mapSound.insert(std::map<TCHAR*,	FMOD_SOUND*>::value_type(pName, pSound));
		else
			delete [] pName;

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	m_Result = FMOD_System_Update(m_pSystem);

}

void CSoundMgr::PlayBgm(TCHAR* szSoundKey)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;
	
	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);
	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0, &m_pBgaCh);
	FMOD_Channel_SetVolume(m_pBgaCh, 0.08f);
}


void CSoundMgr::PlaySound(TCHAR* szSoundKey)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;
	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0, &m_pEffeCh);
}

void CSoundMgr::PlaySkillSound(TCHAR* szSoundKey)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;
	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0, &m_pSkillCh);
}

void CSoundMgr::MonsterSound(TCHAR* szSoundKey)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;
	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0, &m_pMonster);
}

void CSoundMgr::UpdateSound(void)
{
	m_Result = FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopBga(void)
{
	m_Result = FMOD_Channel_Stop(m_pBgaCh);
}

void CSoundMgr::StopSoundAll(void)
{
	m_Result = FMOD_Channel_Stop(m_pEffeCh);
	m_Result = FMOD_Channel_Stop(m_pBgaCh);
	m_Result = FMOD_Channel_Stop(m_pSkillCh);
	m_Result = FMOD_Channel_Stop(m_pMonster);
}

