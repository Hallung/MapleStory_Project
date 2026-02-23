#include "stdafx.h"
#include "MeshRenderer.h"

#include "Resources/Mesh.h"
#include "Resources/Material.h"

//========================================================
// 메시 렌더링 수행
// Mesh / Material 모두 있을 때만 렌더링
// Mesh -> Material -> Topology 순으로 파이프라인 상태 설정
// IndexBuffer 기반 DrawIndexed 호출
//========================================================
void MeshRenderer::Render()
{
	// 렌더링 데이터가 없으면 종료
	if (!mesh || !material) return;

	// 정점/인덱스 버퍼 바인딩 (IA단계)
	mesh->Bind();

	// Shader/InputLayout/Texture 바인딩
	material->Bind();

	// Topology 설정
	Graphics::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(topology);
	// 인덱스 기반 드로우 호출 (IndexCount 만큼 삼각형 생성)
	Graphics::GetInstance().GetDeviceContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}
