#pragma once
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <fmod.hpp>

//==============================================
// 싱글턴 기반 SoundManager 클래스
// 간단한 사운드 재생(BGM / SFX) 기능을 담당
// WinAPI PlaySound를 사용하여 사운드를 비동기로 재생
// BGM 루프 재생 및 효과음 단발 재생을 지원
//==============================================

class SoundManager
{
private:
	SoundManager();
	~SoundManager() = default;

public:
	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}

public:
	// 효과음 재생 (기존 사운드 유지)
	void PlaySFX(const std::wstring& path)
	{
		PlaySound(path.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
	}

	// 배경음 재생 (루프)
	void PlayBGM(const std::wstring& path)
	{
		PlaySound(path.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	// 배경음 정지
	void StopBGM()
	{
		PlaySound(NULL, 0, 0);
	}

	void TestFMOD()
{
    FMOD::System* system = nullptr;
    FMOD::System_Create(&system);

    system->init(32, FMOD_INIT_NORMAL, nullptr);

    system->release();
}
};