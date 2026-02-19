#pragma once

// 최대 입력 키 개수 (전체 가상 키 범위)
#define MAX_INPUT_KEY 256

// 싱글턴 기반 입력 관리 클래스
// 키보드 입력 상태(Donw/Up/Press)를 프레임 단위로 관리
// 마우스 위치 관리

class InputManager
{
private:
	InputManager();
	~InputManager() = default;

public:
	// 싱글턴 복사/이동 금지
	InputManager(const InputManager& other) = delete;
	InputManager& operator=(const InputManager& other) = delete;
	InputManager(InputManager&& other) = delete;
	InputManager& operator=(InputManager&& other) = delete;

	// 싱글턴 인스턴스 반환
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

public:
	// 입력 상태 갱신 (매 프레임 호출)
	// 현재 키 상태 수집, 이전 상태와 비교하여 Down/Up/Press 생성
	void Update();

	// 해당 키가 이번 프레임에 눌린 순간
	bool GetKeyDown(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	// 해당 키가 이번 프레임에 떨어진 순간
	bool GetKeyUp(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	// 해당 키가 현재 눌린 상태 유지
	bool GetKeyPress(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }
	// 현재 마우스의 위치
	DirectX::SimpleMath::Vector2 GetMousePos() const { return mousePos; }

private:
	// 키 입력 상태(열거형)
	enum
	{
		KEY_INPUT_STATUS_NONE = 0,	// 입력X
		KEY_INPUT_STATUS_DOWN,		// 이번 프레임에 눌림
		KEY_INPUT_STATUS_UP,		// 이번 프레임에 떨어짐
		KEY_INPUT_STATUS_PRESS		// 눌린 상태 유지
	};

	// 현재 프레임의 키 상태
	std::array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };
	// 이전 프레임의 키 상태
	std::array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };
	// 프레임 기반 키 상태 (Down/Up/Press 결과)
	std::array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };
	// 현재 마우스 위치
	DirectX::SimpleMath::Vector2 mousePos{};
};