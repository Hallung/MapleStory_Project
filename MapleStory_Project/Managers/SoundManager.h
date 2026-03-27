#pragma once
#include <fmod.hpp>

//==============================================
// 싱글턴 기반 SoundManager 클래스 (FMOD)
// BGM / SFX 동시 재생 지원
// 사운드 로드 및 캐싱, 채널 기반 재생 관리
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
	void Init();
	void Update();

	// 효과음 재생
	void PlaySFX(const std::string& path);
	// 배경음 재생
	void PlayBGM(const std::string& path);

	// 배경음 정지
	void StopBGM();

private:
	// FMOD 시스템 객체
	// 사운드 생성, 재생, 업데이트 등 전체 사운드 관리를 담당
	FMOD::System* system = nullptr;

	// 사운드 캐싱 맵
	// key : 파일 경로
	// value : FMOD::Sound 객체
	// 동일 사운드 중복 생성 방지 및 재사용을 위해 사용
	std::unordered_map<std::string, FMOD::Sound*> soundMap;

	// 현재 재생 중인 BGM 채널
	// BGM 정지 및 제어를 위해 별도로 보관
	FMOD::Channel* bgmChannel = nullptr;

	// 사운드 로드 및 캐싱 함수
	// 이미 로드된 사운드는 재사용하고, 없으면 새로 생성하여 map에 저장
	// loop : true일 경우 루프 재생(BGM), false일 경우 단발 재생(SFX)
	FMOD::Sound* GetOrCreateSound(FMOD::System* system,
		std::unordered_map<std::string, FMOD::Sound*>& map,
		const std::string& path,
		bool loop);
};