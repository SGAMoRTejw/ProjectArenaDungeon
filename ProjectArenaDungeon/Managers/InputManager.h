#pragma once

constexpr size_t MAX_INPUT_KEY = 256;

class InputManager
{
	DECLARE_SINGLETON(InputManager)

public:
	void Update();

	// 키 상태를 반환하는 함수들
	bool GetKeyDown(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool GetKeyUp(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool GetKeyPress(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

	// 마우스 위치 반환 함수
	Vector2 GetMousePos() const { return mousePos; }

private:
	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

	std::array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };		// 현재프레임의 키 눌림/안눌림
	std::array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };	// 이전프레임의 키 눌림/안눌림
	std::array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };		// 키의 상태

	Vector2 mousePos{};
};