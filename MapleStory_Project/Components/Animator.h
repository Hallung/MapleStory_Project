#pragma once
#include "Component.h"

class Texture;

//====================================================
// 애니메이션 클립 데이터 구조
// 하나의 스프라이트 시트 기반 애니메이션 정보 보관
// 텍스처 경로, 프레임 개수, UV 키프레임, 재생 옵션 포함
// Animator에서 재생/제어하기 위한 데이터 컨테이너
//====================================================
struct AnimationClip
{
	//=====================================================
	// Animator가 AnimationClip 내부 데이터에 직접 접근하여
	// 재생/프레임 계산/UV 갱신을 수행할 수 있도록 friend 허용
	//=====================================================
	friend class Animator;

public:
	// 애니메이션 클립 생성
	AnimationClip(
		const std::wstring& name,
		const std::wstring& texturePath,
		UINT frameCount,
		DirectX::SimpleMath::Vector2 startPos,
		DirectX::SimpleMath::Vector2 endPos,
		float playRate = 0.1f,
		bool bLoop = true,
		bool bReverse = false
	);

	// 클립 이름 반환
	std::wstring GetName() const { return name; }

private:
	std::wstring name;						// 애니메이션 이름
	std::wstring texturePath;				// 스프라이트 시드 경로
	std::shared_ptr<Texture> texture;		// 애니메이션 텍스처

	std::vector<DirectX::SimpleMath::Vector2> keyframes;	// 프레임별 UV 좌표
	DirectX::SimpleMath::Vector2 frameSizeUV;				// 한 프레임의 UV 크기

	float playRate;			// 프레임 전환 시간
	bool bLoop;				// 반복 재생 여부
	bool bReverse;			// 역재생 여부
};

// 스프라이트 애니메이션을 제어하는 컴포넌트
// AnimationClip을 등록/재생/전환
// 시간 누적 기반 프레임 진행
class Animator : public Component
{
public:
	Animator() : Component("Animator") {}

	// 매 프레임 애니메이션 업데이트 (시간누적/프레임 인덱스 계산/UV 갱신)
	void Update() override;
	// XML 파일로부터 애니메이션 클립 로드
	void Load(const std::wstring& xmlPath);
	// 애니메이션 클립 등록
	void AddClip(std::shared_ptr<AnimationClip> clip);
	// 클립 재생 시작
	void Play(const std::wstring& clipName);

private:
	std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> clips;	// 클립 목록
	std::shared_ptr<AnimationClip> currentClip;		// 현재 재생 클립

	float accumulatedTime = 0.0f;	// 프레임 전환 누적 시간
	UINT currentFrameIndex = 0;		// 현재 프레임 인덱스
	bool bPlaying = false;			// 재생 상태
};