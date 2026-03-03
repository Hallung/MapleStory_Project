#include "stdafx.h"
#include "TileMap.h"

void TileMap::Update()
{
	//TODO : 타일 애니메이션, 상태 업데이트 등 처리
}

void TileMap::Render()
{
	//TODO : 타일 전체를 일괄 렌더링하도록 구현
}

DirectX::SimpleMath::Vector2 TileMap::WorldToGrid(DirectX::SimpleMath::Vector2 worldPos)
{
	// 월드 좌표를 타일 크기로 나누어 몇 번째 칸인지 계산
	// 음수 좌표에서도 정확한 그리드 인덱스를 얻기 위해 floor 사용
	int x = (int)floor(worldPos.x / tileSize);
	int y = (int)floor(worldPos.y / tileSize);

	return DirectX::SimpleMath::Vector2((float)x, (float)y);
}

DirectX::SimpleMath::Vector2 TileMap::GridToWorld(int gridX, int gridY)
{
	// 그리드 인덱스를 다시 월드 좌표로 변환
	// tileSize를 곱해서 해당 타일의 시작 위치 계산
	float x = gridX * tileSize;
	float y = gridY * tileSize;

	// 타일 중앙 위치를 반환하기 위해 0.5 * tileSize 만큼 보정
	return DirectX::SimpleMath::Vector2(x + tileSize * 0.5f, y + tileSize * 0.5f);
}

bool TileMap::IsValidGrid(int gridX, int gridY)
{
	// 가로 범위 검사
	if (gridX < 0 || gridX >= (int)width) return false;
	// 세로 범위 검사
	if (gridY < 0 || gridY >= (int)height) return false;

	return true;
}
