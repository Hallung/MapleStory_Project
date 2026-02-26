#include "stdafx.h"
#include "ObjectFactory.h"
#include "Objects/Object.h"
#include "Components/MeshRenderer.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/VertexType.h"
#include "Resources/Texture.h"
#include <span>

namespace ObjectFactory
{
namespace
{
//===================================================================
// Objevt에서 MeshRenderer 구성 요소를 생성하여 부착하는 내부 헬퍼 함수
// ObjectFactory 외부에서 직접 사용할 필요가 없어 익명 namespace 사용
//===================================================================
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
	// MeshRenderer 생성 및 메쉬/토폴로지 설정
	auto renderer = std::make_shared<MeshRenderer>();
	renderer->SetMesh(mesh);
	renderer->SetTopology(topology);

	// Material 생성 후 색상 설정
	auto material = std::make_shared<Material>(shaderPath, descs);
	material->SetColor(color);

	// 텍스처 경로가 있으면 TextureManager를 통해 로드 후 설정
	if (texturePath.empty() == false)
	{
		auto texture = TextureManager::GetInstance().LoadTexture(texturePath);
		material->SetTexture(std::move(texture));
	}

	// Renderer에 Material 적용 후 Object에 부착
	renderer->SetMaterial(std::move(material));
	object->AddComponent(renderer);
}
}

//====================================================================
// 2D 스프라이트 Object 생성
// Quad Mesh 와 Texture Shader, 기본 머티리얼을 구성
// texturePath 지정 시 텍스처가 적용된 Sprite Object로 반환
//====================================================================
std::shared_ptr<Object> CreateSprite(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& path)
{
	// Sprite 이름으로 Object 생성
	std::shared_ptr<Object> object = std::make_shared<Object>("Sprite", position, scale, rotation);

	// Quad 메쉬 기반 Sprite 렌더러 구성
	AttachMeshAndMaterial(
		object,
		GeometryHelper::CreateTexturedQuad(),
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		DirectX::SimpleMath::Color(1, 1, 1, 1),	// 기본 색상(텍스처 원본 유지)
		L"_Shaders/Texture.hlsl",
		VertexTexture::descs,
		path
	);

	return object;
}

std::shared_ptr<Object> ObjectFactory::CreateColorRect(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, DirectX::SimpleMath::Color color)
{
	auto obj = std::make_shared<Object>("ColorRect", position, scale, rotation);

	AttachMeshAndMaterial(obj, GeometryHelper::CreateRectangle(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, color, L"_Shaders/Color.hlsl", Color::descs);

	return obj;
}
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

std::shared_ptr<Mesh> CreateRectangle()
{
	static std::shared_ptr<Mesh> mesh;

	if (mesh == nullptr)
	{
		std::vector<Color> vertices(4);
		vertices[0].position = { -0.5f, -0.5f };
		vertices[1].position = { -0.5f, 0.5f };
		vertices[2].position = { 0.5f, -0.5f };
		vertices[3].position = { 0.5f, 0.5f };

		std::vector<UINT> indices = { 0, 1, 2, 2, 1, 3 };

		mesh = std::make_shared<Mesh>();
		mesh->Create(vertices, indices);
	}

	return mesh;
}
}