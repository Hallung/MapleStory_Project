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

	// 실제 타일을 화면에 그리기 위한 프리팹 오브젝트
	// 현재는 테스트 단계라 하나의 오브젝트를 재사용하여 렌더링
	std::shared_ptr<class Object> tilePrefab;

	// 현재 마우스가 위치한 그리드 인덱스
	DirectX::SimpleMath::Vector2 currentGridIndex;
	// 마우스의 월드 좌표
	DirectX::SimpleMath::Vector2 mouseWorldPos;
};