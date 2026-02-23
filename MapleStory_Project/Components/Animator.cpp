#include "stdafx.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "Objects/Object.h"
#include "Renders/ConstantBuffers/GlobalBuffers.h"
#include "Resources/Texture.h"
#include "Resources/Material.h"

#include <tinyxml2.h>

namespace AnimationLoader
{
//============================================
// XML 경로별 AnimationClip 캐시
// 동일 XML 재로드 방지
// 여러 Animator가 같은 애니메이션 데이터를 공유
//============================================
std::unordered_map<std::wstring, std::vector<std::shared_ptr<AnimationClip>>> cache;

//===========================================
// XML 파일에서 AnimationClip 목록 로드
// Texture 단위 XML 구조를 파싱하여 Clip 생성
// 이미 로드된 XML은 캐시 반환
//===========================================
const std::vector<std::shared_ptr<AnimationClip>>& Load(const std::wstring& xmlPath)
{
	// 캐시 조회
	auto it = cache.find(xmlPath);
	if (it != cache.end()) return it->second;

	// 새 클립 목록 생성
	auto& newClips = cache[xmlPath];

	tinyxml2::XMLDocument document;

	// wchar -> char 경로 변환 (tinyxml2는 char 경로 사용)
	std::string path;
	path.reserve(xmlPath.length());
	for (wchar_t wc : xmlPath)
		path.push_back(static_cast<char>(wc));

	//==========================================
	// XML 파일 로드 시도
	// tinyxml2는 XML_SUCCESS가 아니면 로드 실패
	// 실패 시 메시지 박스로 경로와 함께 에러 표시
	// 클립 생성 없이 빈 결과 반환
	//==========================================
	const tinyxml2::XMLError error = document.LoadFile(path.c_str());

	if (error != tinyxml2::XML_SUCCESS)
	{
		// XML 로드 실패 알림
		const std::string message = "XML Load Faild: " + path;
		MessageBoxA(nullptr, message.c_str(), "XML Error", MB_OK);
		return newClips;	// 빈 클립 반환
	}

	// 루트 노드: Texture
	// 하나의 텍스처 시트에 여러 Clip 정의
	const tinyxml2::XMLElement* root = document.FirstChildElement("Texture");
	if (!root) return newClips;

	// 텍스처 경로
	const std::string texPathString = root->Attribute("path");
	const std::wstring texturePath(texPathString.begin(), texPathString.end());

	// Clip 노드 순회
	const tinyxml2::XMLElement* clipNode = root->FirstChildElement("Clip");
	while (clipNode)
	{
		// 클립 이름
		const std::string nameString = clipNode->Attribute("name");
		const std::wstring clipName(nameString.begin(), nameString.end());

		// 스프라이트 시트 내 영역
		const float x = clipNode->FloatAttribute("x");
		const float y = clipNode->FloatAttribute("y");
		const float w = clipNode->FloatAttribute("w");
		const float h = clipNode->FloatAttribute("h");

		// 프레임 수 (기본 1)
		int count = 1;
		if (clipNode->FindAttribute("count")) count = clipNode->IntAttribute("count");

		// 루프 여부 (기본 true)
		bool bLoop = true;
		if (clipNode->FindAttribute("loop")) bLoop = clipNode->BoolAttribute("loop");

		// 재생 속도 (기본 0.1초)
		float speed = 0.1f;
		if (clipNode->FindAttribute("speed")) speed = clipNode->FloatAttribute("speed");

		// 역재생 여부
		bool bReverse = false;
		if (clipNode->FindAttribute("reverse")) bReverse = clipNode->BoolAttribute("reverse");

		// AnimationClip 생성
		auto clip = make_shared<AnimationClip>(
			clipName, texturePath, count, DirectX::SimpleMath::Vector2(x, y),
			DirectX::SimpleMath::Vector2(x + w, y + h), speed, bLoop, bReverse
		);

		newClips.push_back(move(clip));

		// 다음 Clip
		clipNode = clipNode->NextSiblingElement("Clip");
	}

	return newClips;
}
}

//=============================================================
// 애니메이션 클립 생성
// 텍스처에서 지정된 영역(startPos ~ endPos)을 프레임 단위로 분할
// 각 프레임의 시작 UV 좌표와 프레임 크기UV 계산
//=============================================================
AnimationClip::AnimationClip(const std::wstring& name, const std::wstring& texturePath, UINT frameCount, DirectX::SimpleMath::Vector2 startPos, DirectX::SimpleMath::Vector2 endPos, float playRate, bool bLoop, bool bReverse)
	:name(name), texturePath(texturePath), playRate(playRate), bLoop(bLoop), bReverse(bReverse)
{
	// 애니메이션에 사용할 스프라이트 시트 텍스처 로드
	texture = TextureManager::GetInstance().LoadTexture(texturePath);

	// 전체 이미지 크기
	const DirectX::SimpleMath::Vector2 imageSize = texture->GetSize();

	// 애니메이션 영역(pixel 기준)
	const DirectX::SimpleMath::Vector2 clipSize = DirectX::SimpleMath::Vector2(abs(endPos.x - startPos.x), abs(endPos.y - startPos.y));

	// 한 프레임의 pixel 크기
	const DirectX::SimpleMath::Vector2 singleFrameSize = DirectX::SimpleMath::Vector2(clipSize.x / frameCount, clipSize.y);

	// UV 공간 기준 프레임 크기
	frameSizeUV = singleFrameSize / imageSize;

	// 각 프레임 시작 UV 계산
	DirectX::SimpleMath::Vector2 currentPixelPos = startPos;

	//===============================================================
	// 스프라이트 시트의 startPos부터 가로 방향으로 프레임으로 순차 추출
	// 각 프레임의 시작 픽셀 좌표를 UV로 변환하여 keyframes에 저장
	//===============================================================
	for (UINT i = 0; i < frameCount; ++i)
	{
		// 현재 프레임의 시작 UV 좌표
		const DirectX::SimpleMath::Vector2 startUV = currentPixelPos / imageSize;
		keyframes.push_back(startUV);
		// 다음 프레임 위치로 이동 (가로 방향)
		currentPixelPos.x += singleFrameSize.x;
	}
}

//=========================================================
// 애니메이션 프레임 갱신
// 시간 누적 후 playRate 기준으로 프레임 진행
// 루프/정지/역재생 처리
// Material의 Texture 및 FrameBuffer에서 현재 프레임 UV 전달
//=========================================================
void Animator::Update()
{
	// 재생 중이 아니거나 클립이 없으면 종료
	if (currentClip == nullptr || !bPlaying) return;

	// 시간 누적
	accumulatedTime += TimeManager::GetInstance().GetDeltaTime();

	// 프레임 전환 시점 도달 시
	if (accumulatedTime >= currentClip->playRate)
	{
		// 누적 시간을 playRate만큼 차감
		accumulatedTime -= currentClip->playRate;
		++currentFrameIndex;	// currentFrameIndex 증가

		// 마지막 프레임 도달 시
		if (currentFrameIndex >= currentClip->keyframes.size())
		{
			if (currentClip->bLoop)
			{
				currentFrameIndex = 0;	// 루프
			}
			else
			{
				currentFrameIndex = (UINT)currentClip->keyframes.size() - 1;
				bPlaying = false;	// 재생 종료
			}
		}
	}

	// MeshRenderer의 Material 가져오기
	const auto& material = owner->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial();
	if (material == nullptr) return;	// 가져오지 못했으면 무시

	// 애니메이션 텍스처 적용
	material->SetTexture(currentClip->texture);

	// Material이 보유한 FrameBuffer 가져오기
	const auto& frameBuffer = material->GetFrameBuffer();
	if (frameBuffer)
	{
		// 기본적으로 현재 프레임 인덱스를 사용
		UINT realFrameIndex = currentFrameIndex;

		// 역재생 옵션이 활성화된 경우 프레임 순서를 반대로 계산
		if(currentClip->bReverse)
			realFrameIndex = (UINT)currentClip->keyframes.size() - 1 - currentFrameIndex;

		// 현재 프레임의 UV 시작 좌표와 UV 크기
		const DirectX::SimpleMath::Vector2 startUV = currentClip->keyframes[realFrameIndex];
		const DirectX::SimpleMath::Vector2 sizeUV = currentClip->frameSizeUV;

		// 셰이더에서 사용할 UV 영역을 전달
		frameBuffer->SetFrameData(startUV, sizeUV);
	}
}

// XML 파일에서 애니메이션 클립 로드 후 Animator에 등록
void Animator::Load(const std::wstring& xmlPath)
{
	const auto& clipsFromFile = AnimationLoader::Load(xmlPath);

	for (const auto& clip : clipsFromFile)
		AddClip(clip);
}

//=======================================
// Animator에 애니메이션 클립 추가
// 이름을 키로 저장하여 Play에서 조회 가능
//=======================================
void Animator::AddClip(std::shared_ptr<AnimationClip> clip)
{
	if (clip == nullptr) return;
	clips[clip->GetName()] = move(clip);
}

//==================================
// 지정한 이름의 애니메이션 재생 시작
// 동일 클립이 이미 재생 중이면 무시
//==================================
void Animator::Play(const std::wstring& clipName)
{
	// 요청한 이름의 애니메이션 클립 검색
	auto it = clips.find(clipName);
	if (it == clips.end()) return;	// 없으면 무시

	const auto& nextClip = it->second;

	// 이미 같은 클립 재생 중이면 상태 유지
	if (currentClip == nextClip && bPlaying) return;

	// 재생할 클립으로 전환
	currentClip = nextClip;

	// 프레임 및 시간 누적값 초기화 -> 애니메이션을 처음부터 재생
	currentFrameIndex = 0;
	accumulatedTime = 0.0f;

	// 재생 상태 활성화
	bPlaying = true;
}