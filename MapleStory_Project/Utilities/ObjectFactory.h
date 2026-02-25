#pragma once

class Object;
class Mesh;

//==============================================================================
// Object 생성 편의 함수 네임 스페이스
// 실제 Factory 패턴 구현이 아닌 GeometryHelper의 형태로 Object를 만들어 내는 구조
// Transform data, Mesh, MeshRenderer를 shared_ptr<Object> 형태로 return
//==============================================================================
namespace ObjectFactory
{
//=================================================================================
// 2D 스프라이트 Object 생성
// 위치/스케일/회전을 설정하고, 텍스처 경로가 지정된 경우 Sprite용 머티리얼을 함께 구성
//=================================================================================
std::shared_ptr<Object> CreateSprite(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation = 0.0f, const std::wstring& path = L"");
std::shared_ptr<Object> CreateColorRect(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation = 0.0f, DirectX::SimpleMath::Color color = {1, 0, 0, 1});
}

//=======================================================
// 기본 메시(기하) 생성 유틸리티 네임스페이스
// 렌더링에서 자주 사용하는 메시를 간단히 생성하기 위한 헬퍼
//=======================================================
namespace GeometryHelper
{
// 텍스처 좌표가 포함된 Quad 메시 생성
std::shared_ptr<Mesh> CreateTexturedQuad();
std::shared_ptr<Mesh> CreateRectangle();
}