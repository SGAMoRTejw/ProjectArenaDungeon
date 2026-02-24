#include "pch.h"
#include "InputManager.h"

InputManager::InputManager() {}

void InputManager::Update()
{
	// 1. 이전 프레임 키 상태 백업
	keyOldState = keyState;

	// 2. 현재 프레임 키 상태 수집 (WinAPI)
	// GetKeyboardStatesms 256개 키의 상태를 반환 (최상위 비트: 눌림 여부)
	if (GetKeyboardState(keyState.data()))
	{
		for (size_t i = 0; i < MAX_INPUT_KEY; ++i)
		{
			// 3. keyState를 0, 1 값으로 설정
			keyState[i] = keyState[i] & 0x80 ? 1 : 0;
			const auto& oldState = keyOldState[i];
			const auto& state = keyState[i];

			// 4. 이전 상태 / 현재 상태 비교로 입력 이벤트 생성
			if (oldState == 0 && state == 1)
			{
				keyMap[i] = KEY_INPUT_STATUS_DOWN;
			}
			else if (oldState == 1 && state == 0)
			{
				keyMap[i] = KEY_INPUT_STATUS_UP;
			}
			else if (oldState == 1 && state == 1)
			{
				keyMap[i] = KEY_INPUT_STATUS_PRESS;
			}
			else
			{
				keyMap[i] = KEY_INPUT_STATUS_NONE;
			}
		}
	}

	// 5. 마우스 좌표 갱신
	// - ScreenToClient로 클라이언트 좌표로 변환
	// - 좌하단 원점 좌표계를 사용하기 위해 y축을 반전한다.
	POINT pt;
	GetCursorPos(&pt);
	assert(gHandle && "InputManager::Update() requires valid window handle(gHandle)");
	ScreenToClient(gHandle, &pt);

	mousePos = {
		static_cast<float>(pt.x),
		static_cast<float>(gWinHeight - pt.y)
	};
}