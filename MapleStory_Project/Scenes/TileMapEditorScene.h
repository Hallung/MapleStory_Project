#pragma once
#include "Scene.h"

// TileMap 구조 테스트 및 좌표 변환 확인용 Scene
class TileMapEditorScene : public Scene
{
public:
	void Init() override;
	void Destroy() override;
	void Update() override;
	void Render() override;

private:
	// 테스트용 TileMap
	std::shared_ptr<class TileMap> tileMap;
	// 현재 선택된 타일 위치를 표시하기 위한 커서 오브젝트
	std::shared_ptr<class Object> cursorObject; 

	// 현재 마우스가 위치한 그리드 인덱스
	DirectX::SimpleMath::Vector2 currentGridIndex;
	// 마우스의 월드 좌표
	DirectX::SimpleMath::Vector2 mouseWorldPos;
};