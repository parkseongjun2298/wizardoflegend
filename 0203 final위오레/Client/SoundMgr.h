#pragma once
class CSoundMgr
{
public:
	static CSoundMgr* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum CHANNELID { BGM, PLAYER, PLAYEREFFECT, MONSTER, BOSSEFFECT, BOSS, EFFECT, UI, EXAMPLE, MAXCHANNEL };
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();

	void Release();
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlaySoundVolume(TCHAR* pSoundKey, CHANNELID eID, float vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetVolume(CHANNELID eID, float vol);
	void Update() { FMOD_System_Update(m_pSystem); }//���尡 ����Ǵ� ���� ����Ʈ������Ʈ �� �������� ���� ȣ�����ָ�


private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

};

