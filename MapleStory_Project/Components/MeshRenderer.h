#pragma once
#include "Component.h"

class Mesh;
class Material;

// 메시 렌더링 컴포넌트
// Mesh와 Material을 이용해 오브젝트를 화면에 그리는 Renderer
// IA 단계 Topology 설정 후 Mesh버퍼 바인딩
class MeshRenderer : public Component
{
public:
	// 기본 이름 "MeshRenderer"
	MeshRenderer(const std::string& name = "MeshRenderer") : Component(name) {}
	~MeshRenderer() override = default;

	// 렌더링 수행
	// Topology 설정, Mesh 바인딩 후 Draw호출, Material 바인딩
	void Render() override;

	// 렌더링에 사용할 메시 설정
	void SetMesh(std::shared_ptr<Mesh> mesh) { this->mesh = move(mesh); }
	// 렌더링에 사용할 머티리얼 설정
	void SetMaterial(std::shared_ptr<Material> material) { this->material = move(material); }
	// IA 단계 Primitive Topology 설정
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { this->topology = topology; }

	// 현재 머티리얼 반환
	const std::shared_ptr<Material>& GetMaterial() const { return material; }

private:
	std::shared_ptr<Mesh> mesh;	// Mesh 데이터
	std::shared_ptr<Material> material;	// Material 데이터
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// Topology 상태
};