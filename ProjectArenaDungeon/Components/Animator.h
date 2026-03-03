#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "_Libraries/DirectXTK/SimpleMath.h"

#include "Components/Component.h"

class Texture;

// AnimationClip
// - 텍스처 아틀라스에서 특정 구간을 frameCount로 쪼개 keyframe(시작 UV) 목록을 생성
// - Animator가 재생 시 frameBuffer에 (startUV, sizeUV)를 전달
class AnimationClip
{
	friend class Animator;

public:
	AnimationClip(
		std::wstring name,
		std::wstring texturePath,
		unsigned int frameCount,
		DirectX::SimpleMath::Vector2 startPosPixel,
		DirectX::SimpleMath::Vector2 endPosPixel,
		float playRate = 0.1f,
		bool bLoop = true,
		bool bReverse = false
	);

	const std::wstring& GetName() const { return name; }

private:
	std::wstring name;
	std::wstring texturePath;

	std::shared_ptr<Texture> texture;

	// keyframes
	// - 각 프레임의 시작 UV (0~1)
	std::vector<DirectX::SimpleMath::Vector2> keyframes;

	// frameSizeUV
	// - 단일 프레임의 UV 크기 (0~1)
	DirectX::SimpleMath::Vector2 frameSizeUV{ 1.0f, 1.0f };

	float playRate = 0.1f;
	bool bLoop = true;
	bool bReverse = false;
};

// Animator
// - Clip을 관리하고 현재 재생 프레임을 Material(FrameBuffer)에 반영
// - 시간 누적 기반으로 프레임을 전진시키며, 역재생(bReverse)/반복(loop) 옵션을 지원
class Animator final : public Component
{
public:
	Animator();

	void Update() override;

	// Load
	// - xmlPath에 정의된 Clip 목록을 로드하여 AddClip 수행
	void Load(const std::wstring& xmlPath);

	void AddClip(std::shared_ptr<AnimationClip> clip);
	void Play(const std::wstring& clipName);

	const std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>>& GetClips() const { return clips; }
	const std::shared_ptr<AnimationClip>& GetCurrentClip() const { return currentClip; }

	unsigned int GetCurrentFrameIndex() const { return currentFrameIndex; }
	void SetCurrentFrameIndex(unsigned int index) { currentFrameIndex = index; }

	bool IsPlaying() const { return bPlaying; }
	void SetPlaying(bool playing) { bPlaying = playing; }

	unsigned int GetCurrentClipFrameCount() const
	{
		return currentClip ? static_cast<unsigned int>(currentClip->keyframes.size()) : 0;
	}

private:
	void ApplyFrameToMaterial();

private:
	std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> clips;
	std::shared_ptr<AnimationClip> currentClip;

	float accumulatedTime = 0.0f;
	unsigned int currentFrameIndex = 0;
	bool bPlaying = false;
};