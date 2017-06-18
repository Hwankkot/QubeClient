#pragma once

class CSoundMgr
{
private:
	FMOD_SYSTEM*	m_pSystem;	//FMOD	system pointer
	FMOD_CHANNEL*	m_pEffeCh;	//이펙트 사운드 채널
	FMOD_CHANNEL*	m_pBgaCh;	//메인 BGM 채널
	FMOD_CHANNEL*	m_pSkillCh;	//스킬 플레이 채널
	FMOD_CHANNEL*	m_pMonster;	//몬스터 사운드 플레이 채널
	FMOD_RESULT		m_Result;	//FMOD	Running	Result

private:
	unsigned	int		m_iVersion;
	std::map<TCHAR*,	FMOD_SOUND*>		m_mapSound;	//사운드들을 가지고 있는 map컨테이너
public:
	std::map<TCHAR*,	FMOD_SOUND*>*	GetSoundMap(void)
	{
		return &m_mapSound;
	}
public:
	void	Initialize(void);
	void	LoadSoundFile(void);
	void	PlayBgm(TCHAR*	szSoundKey);
	void	PlaySound(TCHAR*	szSoundKey);
	void	PlaySkillSound(TCHAR*	szSoundKey);
	void	MonsterSound(TCHAR*	szSoundKey);
	void	UpdateSound(void);
	void	StopBga(void);
	void	StopSoundAll(void);

private:
	static	CSoundMgr*	m_pInstance;
public:
	static	CSoundMgr*	GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	void	DestroyInstance(void)
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
private:
	CSoundMgr(void);
public:
	~CSoundMgr(void);
};
