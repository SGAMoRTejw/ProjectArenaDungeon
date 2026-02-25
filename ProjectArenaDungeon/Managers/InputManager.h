#pragma once

// NOTE: key는 0 ~ MAX_INPUT_KEY-1 범위 여야 한다.
constexpr size_t MAX_INPUT_KEY = 256;

// InputManager
// - 키보드 / 마우스 입력 상태를 프레임 단위로 갱신하여 Down, Up, Press 상태로 제공
// - Update()는 매 프레임 1회 호출

class InputManager
{
	DECLARE_SINGLETON(InputManager)

public:
	// Update
	// - 현재 프레임의 입력 상태를 수집하고, 이전 프레임과 비교하여 Down, Up, Press 상태를 저장
	void Update();

	// 키 상태 조회
	// - Down	: 이번 프레임에 눌림
	// - Up		: 이번 프레임에 떼어짐
	// - Press	: 누르고 있는 중
	bool GetKeyDown(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool GetKeyUp(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool GetKeyPress(size_t key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

	// GetMousePos
	// - 현재 마우스 좌표
	DirectX::SimpleMath::Vector2 GetMousePos() const { return mousePos; }

private:
	// 키 상태 정의
	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

	// Raw key state (현재 / 이전 프레임)
	std::array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };
	std::array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };
	// Mapped input state (Down / Up / Press)
	std::array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };

	// 마우스 좌표
	DirectX::SimpleMath::Vector2 mousePos{};
};