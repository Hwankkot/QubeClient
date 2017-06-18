#pragma once

class CSoundMgr
{
private:
	FMOD_SYSTEM*	m_pSystem;	//FMOD	system pointer
	FMOD_CHANNEL*	m_pEffeCh;	//����Ʈ ���� ä��
	FMOD_CHANNEL*	m_pBgaCh;	//���� BGM ä��
	FMOD_CHANNEL*	m_pSkillCh;	//��ų �÷��� ä��
	FMOD_CHANNEL*	m_pMonster;	//���� ���� �÷��� ä��
	FMOD_RESULT		m_Result;	//FMOD	Running	Result

private:
	unsigned	int		m_iVersion;
	std::map<TCHAR*,	FMOD_SOUND*>		m_mapSound;	//������� ������ �ִ� map�����̳�
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
