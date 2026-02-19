#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager() {}

void InputManager::Update()
{
	// 이전 프레임의 키 상태 받아오기
	keyOldState = keyState;

	// 현재 키보드 상태 적용
	if (GetKeyboardState(keyState.data()))
	{
		for (size_t i = 0; i < MAX_INPUT_KEY; ++i)
		{
			// 최상위 비트(0x80)의 키 눌림 여부
			// 눌림 : 1, 안 눌림 : 0
			keyState[i] = keyState[i] & 0x80 ? 1 : 0;

			const auto& oldState = keyOldState[i];	// 이전 프레임 상태
			const auto& state = keyState[i];		// 현재 프레임 상태

			// 키 입력 상태 갱신 
			if (oldState == 0 && state == 1)
				keyMap[i] = KEY_INPUT_STATUS_DOWN;
			else if (oldState == 1 && state == 0)
				keyMap[i] = KEY_INPUT_STATUS_UP;
			else if (oldState == 1 && state == 1)
				keyMap[i] = KEY_INPUT_STATUS_PRESS;
			else
				keyMap[i] = KEY_INPUT_STATUS_NONE;
		}
	}
	// 마우스 커서 화면 좌표 얻기
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);

	// 클라이언트 좌표계로 변환 (윈도우 기준)
	ScreenToClient(gHandle, &cursorPoint);

	// 엔진 좌표계로 변환(원점: 좌하단, 윈도우: 좌상단 원점 -> Y축 뒤집기)
	mousePos = { (float)cursorPoint.x, gWinHeight - (float)cursorPoint.y };
}