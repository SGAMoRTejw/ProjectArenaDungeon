#include "pch.h"
#include "InputManager.h"

InputManager::InputManager() {}

void InputManager::Update()
{
	keyOldState = keyState;

	if (GetKeyboardState(keyState.data()))
	{
		for (size_t i = 0; i < MAX_INPUT_KEY; ++i)
		{
			keyState[i] = keyState[i] & 0x80 ? 1 : 0;
			const auto& oldState = keyOldState[i];
			const auto& state = keyState[i];

			// 키의 이전 상태와 현재 상태를 비교하여 키의 상태를 저장
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

	POINT pt;

	GetCursorPos(&pt);

	ScreenToClient(gHandle, &pt);

	mousePos = {
		static_cast<float>(pt.x),
		static_cast<float>(gWinHeight - pt.y)
	};
}