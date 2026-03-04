#pragma once

// 단순한 순수 데이터 구조체
// 한 칸의 타일이 가지는 최소 정보만 보관
struct TileInfo
{
	int textureIndex = -1; // -1이면 비어있는 타일, 0 이상이면 타일이 존재한다고 간주
	bool bSolid = false; // 충돌 여부 판단용
};

// 타일 기반 맵의 기본 구조를 담당하는 클래스
class TileMap
{
public:
	// width  : 가로 타일 개수
	// height : 세로 타일 개수
	// tileSize : 타일 한 칸의 월드 좌표 크기
	TileMap(UINT width, UINT height, float tileSize);

	void Update();
	void Render();

	// 월드 좌표를 그리드 인덱스로 변환
	DirectX::SimpleMath::Vector2 WorldToGrid(DirectX::SimpleMath::Vector2 worldPos) const;

	// 그리드 인덱스를 월드 좌표(타일 중심)로 변환
	DirectX::SimpleMath::Vector2 GridToWorld(int gridX, int gridY) const;

	// 해당 인덱스가 맵 범위 안에 있는지 검사
	bool IsValidGrid(int gridX, int gridY) const;

	// 특정 위치의 타일 정보를 설정
	void SetTile(int gridX, int gridY, int textureIndex);

	// 타일 정보 접근 함수
	TileInfo* GetTile(int gridX, int gridY);
	const TileInfo* GetTile(int gridX, int gridY) const;

	UINT GetWidth() const { return width; }
	UINT GetHeight() const { return height; }
	float GetTileSize() const { return tileSize; }

private:
	UINT width = 0; // 맵 가로 타일 수
	UINT height = 0; // 맵 세로 타일 수
	float tileSize = 0.0f; // 타일 한 칸의 크기 (월드 단위)

	// 2차원 타일 데이터를 1차원 배열로 저장 (index = y * width + x 방식 사용)
	std::vector<TileInfo> tiles;
};
