#include "stdafx.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "Objects/Object.h"
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

AnimationClip::AnimationClip(const std::wstring& name, const std::wstring& texturePath, UINT frameCount, DirectX::SimpleMath::Vector2 startPos, DirectX::SimpleMath::Vector2 endPos, float playRate, bool bLoop, bool bReverse)
	:name(name), texturePath(texturePath), playRate(playRate), bLoop(bLoop), bReverse(bReverse)
{
	// TODO : TextureManager 제작 후 작성 
}
