#pragma once


// 타일 기반 맵의 기본 구조를 담당하는 클래스
class TileMap
{
public:
	// width  : 가로 타일 개수
	// height : 세로 타일 개수
	// tileSize : 타일 한 칸의 월드 좌표 크기
	TileMap(UINT width, UINT height, float tileSize) : width(width), height(height), tileSize(tileSize) {}

	void Update();
	void Render();

	// 월드 좌표를 그리드 인덱스로 변환
	DirectX::SimpleMath::Vector2 WorldToGrid(DirectX::SimpleMath::Vector2 worldPos);

	// 그리드 인덱스를 월드 좌표(타일 중심)로 변환
	DirectX::SimpleMath::Vector2 GridToWorld(int gridX, int gridY);

	// 해당 인덱스가 맵 범위 안에 있는지 검사
	bool IsValidGrid(int gridX, int gridY);

	UINT GetWidth() const { return width; }
	UINT GetHeight() const { return height; }
	float GetTileSize() const { return tileSize; }

private:
	UINT width = 0; // 맵 가로 타일 수
	UINT height = 0; // 맵 세로 타일 수
	float tileSize = 0.0f; // 타일 한 칸의 크기 (월드 단위)
};
