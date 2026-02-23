#include "stdafx.h"
#include "ObjectFactory.h"
#include "Objects/Object.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/VertexType.h"
#include <span>

namespace ObjectFactory
{
namespace
{
// Objevt에서 MeshRenderer 구성 요소를 생성하여 부착하는 내부 헬퍼 함수
void AttachMeshAndMaterial(
	const std::shared_ptr<Object>& object,
	const std::shared_ptr<Mesh>& mesh,
	D3D11_PRIMITIVE_TOPOLOGY topology,
	DirectX::SimpleMath::Color color,
	const std::wstring& shaderPath,
	std::span<const D3D11_INPUT_ELEMENT_DESC> descs,
	const std::wstring& texturePath = L""
)
{
	// TODO : 
	// MeshRenderer 생성 후 Object에 추가하는 공통 로직 구현 예정
	// 예정 동작:
	// 1. MeshRenderer 생성
	// 2. Mesh / Topology 설정
	// 3. Material 생성 후 Color 설정
	// 4. texturePath가 있으면 Texture로 로드 후 Material에 설정
	// 5. Renderer를 Object에 Component로 추가
}
}

//====================================================================
// 2D 스프라이트 Object 생성
// Quad Mesh 와 머티리얼을 구성하여 Sprite 렌더링이 가능한 Object로 반환
//====================================================================
//std::shared_ptr<Object> CreateSprite(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation = 0.0f, const std::wstring& path = L"")
//{
//	// Sprite 이름으로 Object 생성
//	std::shared_ptr<Object> object = std::make_shared<Object>("Sprite", position, scale, rotation);
//
//	// TODO : AttachMeshAndMaterial 구현 후 다음 구성 예정
//}
}

namespace GeometryHelper
{
//===================================================
// 텍스처 좌표가 포함된 Quad Mesh를 생성한다
// 내부적으로 한 번만 생성하여 재사용되는 공유 Mesh 사용
//===================================================
std::shared_ptr<Mesh> CreateTexturedQuad()
{
	// 정적 캐시 : 최초 1회 생성 후 재사용
	static std::shared_ptr<Mesh> mesh = nullptr;

	if (mesh == nullptr)
	{
		// 2D Quad 정점 4개 생성
		std::vector<VertexTexture> vertices(4);
		vertices[0].position = { -0.5f, -0.5f };
		vertices[1].position = { -0.5f, 0.5f };
		vertices[2].position = { 0.5f, -0.5f };
		vertices[3].position = { 0.5f, 0.5f };

		// 텍스처 UV 좌표 (DirectX 기준 Y축 뒤집힘 고려)
		vertices[0].uv = { 0.0f, 1.0f };
		vertices[1].uv = { 0.0f, 0.0f };
		vertices[2].uv = { 1.0f, 1.0f };
		vertices[3].uv = { 1.0f, 0.0f };

		// 두개의 삼각형으로 Quad 구성
		std::vector<UINT> indices = { 0, 1, 2, 2, 1, 3 };

		// Mesh 생성 및 버퍼 구성
		mesh = std::make_shared<Mesh>();
		mesh->Create(vertices, indices);
	}

	return mesh;
}
}