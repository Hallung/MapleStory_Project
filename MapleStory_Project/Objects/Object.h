#pragma once

class Component;
class Transform;

// 게임 내 존재하는 모든 객체의 기본 클래스
class Object
{
public:
	Object(const std::string& name, DirectX::SimpleMath::Vector2 position = { 0, 0 }, DirectX::SimpleMath::Vector2 scale = { 1, 1 }, float rotation = 0.0f);
	virtual ~Object() = default;

	// Object 생명주기 함수
	// Object가 소유한 모든 Component에게 전달됨
	virtual void Awake(); // 초기화 단계에서 호출
	virtual void Update(); // 매 프레임 호출
	virtual void Render(); // 렌더링 단계에서 호출

	// Component를 Object에 등록하고 Owner 설정
	void AddComponent(const std::shared_ptr<Component>& component);

	// 이름 기반으로 Component 반환
	template<typename T>
	std::shared_ptr<T> GetComponent(const std::string& compName) const
	{
		if (!components.count(compName))
			return nullptr;

		// dynamic_pointer_cast를 사용하여 안전하게 타입 변환
		return dynamic_pointer_cast<T>(components.at(compName));
	}

	Transform* GetTransform() { return transform.get(); } // Object는 Transform을 항상 보유, 별도 검색 없이 빠르게 접근하기 위한 전용 Getter

protected:
	std::string name; // Object 이름
	std::shared_ptr<Transform> transform; // Object의 공간 정보(위치, 회전, 스케일) 담당, 항상 존재하는 핵심 Component

	// 이름 기반 Component 저장 및 조회용
	std::unordered_map<std::string, std::shared_ptr<Component>> components;
	// Update / Render 순회용 리스트, 순서 기반 실행을 위해 별도 관리
	std::vector<std::shared_ptr<Component>> updateList;
};