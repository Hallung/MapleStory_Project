#include "stdafx.h"
#include "SoundManager.h"
#include "Utilities/PathUtils.h"

SoundManager::SoundManager() {}

// FMOD 시스템 초기화
// 최대 채널 수 설정 및 기본 설정으로 초기화
void SoundManager::Init()
{
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, nullptr);
}

// FMOD 시스템 업데이트
// 내부 사운드 상태 및 스트리밍 갱신 (매 프레임 호출)
void SoundManager::Update()
{
	system->update();
}

// 효과음 재생 (SFX)
// 루프 없이 단발성으로 재생
void SoundManager::PlaySFX(const std::string& path)
{
	std::string fullPath = PathUtils::GetFullPathS(path);

	FMOD::Sound* sound = GetOrCreateSound(system, soundMap, fullPath, false);

	FMOD::Channel* channel = nullptr;
	system->playSound(sound, nullptr, false, &channel);
}

// 배경음 재생 (BGM)
// 루프 옵션으로 재생되며 bgmChannel에 저장
void SoundManager::PlayBGM(const std::string& path)
{
	std::string fullPath = PathUtils::GetFullPathS(path);

	FMOD::Sound* sound = GetOrCreateSound(system, soundMap, fullPath, true);

	system->playSound(sound, nullptr, false, &bgmChannel);
}

// 현재 재생 중인 BGM 정지
void SoundManager::StopBGM()
{
	if (bgmChannel)
	{
		bgmChannel->stop();
	}
}

// 사운드 생성 및 캐싱 함수
// 이미 존재하는 사운드는 재사용하고, 없을 경우 새로 생성하여 map에 저장
// loop 여부에 따라 루프 설정 적용
FMOD::Sound* SoundManager::GetOrCreateSound(FMOD::System* system, std::unordered_map<std::string, FMOD::Sound*>& map, const std::string& path, bool loop)
{
	std::string fullPath = PathUtils::GetFullPathS(path);

	// 이미 로드된 사운드가 존재하면 반환
	if (map.find(fullPath) != map.end())
		return map[fullPath];

	FMOD::Sound* sound = nullptr;

	// 사운드 생성 (루프 여부에 따라 옵션 설정)
	system->createSound(
		fullPath.c_str(),
		loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT,
		nullptr,
		&sound
	);

	// 생성된 사운드를 캐싱
	map[fullPath] = sound;
	return sound;
}
