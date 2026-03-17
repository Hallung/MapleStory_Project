#pragma once
#include "stdafx.h"
#include "ConstantBuffer.h"

// World 행렬 데이터 구조체
// Object의 위치, 회전, 스케일 정보를 Shader로 전달할 때 사용
struct WorldData
{
	DirectX::SimpleMath::Matrix world;
};

// Object의 World 행렬을 Shader에 전달하는 ConstantBuffer
class WorldBuffer : public ConstantBuffer<WorldData>
{
public:
	WorldBuffer() : ConstantBuffer() {}

	// World 행렬 설정
	// Shader는 Column Major 방식을 사용하므로 Transpose 해서 저장
	void SetWorld(const DirectX::SimpleMath::Matrix& world)
	{
		data.world = world.Transpose();

		bDirty = true; // 데이터 변경됨 표시
	}
};

// View / Projection 행렬 데이터 구조체
// Camera 정보 전달에 사용
struct ViewProjectionData
{
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;
};

// Camera의 View / Projection 행렬을 Shader에 전달하는 ConstantBuffer
class ViewProjectionBuffer : public ConstantBuffer<ViewProjectionData>
{
public:
	ViewProjectionBuffer() : ConstantBuffer() {}

	// View 행렬 설정 (카메라 위치, 방향)
	void SetView(const DirectX::SimpleMath::Matrix& view)
	{
		data.view = view.Transpose();

		bDirty = true;
	}

	// Projection 행렬 설정 (원근 / 직교 투영)
	void SetProjection(const DirectX::SimpleMath::Matrix& projection)
	{
		data.projection = projection.Transpose();

		bDirty = true;
	}
};

// Color 데이터 구조체
// Object 색상 정보 전달에 사용
struct ColorData
{
	DirectX::SimpleMath::Color color;
};

// Object의 색상을 Shader에 전달하는 ConstantBuffer
class ColorBuffer : public ConstantBuffer<ColorData>
{
public:
	ColorBuffer() : ConstantBuffer() {}

	// Color 설정
	// 동일한 값이면 업데이트하지 않아 불필요한 GPU 업데이트 방지
	void SetColor(DirectX::SimpleMath::Color color)
	{
		if (data.color == color)
			return;

		data.color = color;
		bDirty = true;
	}

	DirectX::SimpleMath::Color GetColor() const { return data.color; }
};

// Sprite Animation에서 사용할 UV Frame 데이터 구조체
// Texture에서 현재 사용할 영역 지정
struct FrameData
{
	DirectX::SimpleMath::Vector2 startUV; // 시작 UV 좌표
	DirectX::SimpleMath::Vector2 sizeUV; // UV 크기
};

// Sprite Animation UV 정보를 Shader에 전달하는 ConstantBuffer
class FrameBuffer : public ConstantBuffer<FrameData>
{
public:
	FrameBuffer() : ConstantBuffer() {}

	// UV Frame 데이터 설정
	// 동일한 값이면 업데이트하지 않아 성능 최적화
	void SetFrameData(DirectX::SimpleMath::Vector2 start, DirectX::SimpleMath::Vector2 size)
	{
		if (data.startUV == start && data.sizeUV == size)
			return;

		data.startUV = start;
		data.sizeUV = size;
		bDirty = true;
	}
};

// Sprite Atlas 데이터 구조체
// Atlas 기반 Sprite Instancing에서 텍스처 전체 크기와 Sprite 단위 크기 전달
struct SpriteAtlasData
{
	DirectX::SimpleMath::Vector2 textureSize; // Atlas 전체 텍스처 크기
	DirectX::SimpleMath::Vector2 spriteSize;  // 개별 Sprite 크기
};

// Sprite Atlas 정보를 Shader에 전달하는 ConstantBuffer
// instIndex를 이용해 Atlas UV 계산에 사용
class SpriteAtlasBuffer : public ConstantBuffer<SpriteAtlasData>
{
public:
	// Atlas 데이터 설정
	// 동일한 값이면 업데이트하지 않아 GPU 업데이트 최소화
	void SetData(DirectX::SimpleMath::Vector2 textureSize, DirectX::SimpleMath::Vector2 spriteSize)
	{
		if (data.textureSize == textureSize && data.spriteSize == spriteSize)
			return;

		data.textureSize = textureSize;
		data.spriteSize = spriteSize;
		bDirty = true;
	}
};