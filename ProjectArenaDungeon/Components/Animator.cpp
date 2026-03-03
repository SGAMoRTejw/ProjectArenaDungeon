#include "pch.h"
#include "Components/Animator.h"

#include <algorithm>
#include <string>

#include <tinyxml2.h>

#include "Config/ConstantValues.h"

#include "Objects/Object.h"

#include "Renders/ConstantBuffers/GlobalBuffers.h"

#include "Resources/Texture.h"
#include "Resources/Material.h"

#include "Components/MeshRenderer.h"
#include "Managers/TextureManager.h"   // TEXTURES
#include "Managers/TimeManager.h"      // TIME

using Vector2 = DirectX::SimpleMath::Vector2;

namespace
{
// NOTE: tinyxml2는 char* 기반 경로를 사용하므로 wstring -> utf8 변환이 필요.
//       실습처럼 wchar를 char로 강제 캐스팅하면 (한글/특수문자 경로)에서 깨질 수 있다.
std::string ToUtf8(const std::wstring& ws)
{
	if (ws.empty()) return {};

	const int sizeNeeded = ::WideCharToMultiByte(CP_UTF8, 0, ws.data(),
		static_cast<int>(ws.size()), nullptr, 0, nullptr, nullptr);

	std::string result;
	result.resize(sizeNeeded);

	::WideCharToMultiByte(CP_UTF8, 0, ws.data(), static_cast<int>(ws.size()),
		result.data(), sizeNeeded, nullptr, nullptr);

	return result;
}

// AnimationLoader
// - xmlPath 기준 Clip 목록 캐싱
namespace AnimationLoader
{
std::unordered_map<std::wstring, std::vector<std::shared_ptr<AnimationClip>>> cache;

const std::vector<std::shared_ptr<AnimationClip>>& Load(const std::wstring& xmlPath)
{
	if (auto it = cache.find(xmlPath); it != cache.end())
		return it->second;

	auto& newClips = cache[xmlPath];

	tinyxml2::XMLDocument doc;
	const std::string pathUtf8 = ToUtf8(xmlPath);

	const tinyxml2::XMLError error = doc.LoadFile(pathUtf8.c_str());
	if (error != tinyxml2::XML_SUCCESS)
	{
		const std::string msg = "XML Load Failed: " + pathUtf8;
		::MessageBoxA(nullptr, msg.c_str(), "XML Error", MB_OK);
		return newClips;
	}

	const tinyxml2::XMLElement* root = doc.FirstChildElement("Texture");
	if (!root)
		return newClips;

	const char* texPathCStr = root->Attribute("path");
	if (!texPathCStr)
		return newClips;

	// NOTE: xml의 path는 일반적으로 ASCII/UTF8로 작성되므로 단순 변환.
	const std::string texPathStr = texPathCStr;
	const std::wstring texturePath(texPathStr.begin(), texPathStr.end());

	const tinyxml2::XMLElement* clipNode = root->FirstChildElement("Clip");
	while (clipNode)
	{
		const char* nameCStr = clipNode->Attribute("name");
		if (!nameCStr)
		{
			clipNode = clipNode->NextSiblingElement("Clip");
			continue;
		}

		const std::string nameStr = nameCStr;
		const std::wstring clipName(nameStr.begin(), nameStr.end());

		const float x = clipNode->FloatAttribute("x");
		const float y = clipNode->FloatAttribute("y");
		const float w = clipNode->FloatAttribute("w");
		const float h = clipNode->FloatAttribute("h");

		int count = 1;
		if (clipNode->FindAttribute("count"))
			count = clipNode->IntAttribute("count");

		bool bLoop = true;
		if (clipNode->FindAttribute("loop"))
			bLoop = clipNode->BoolAttribute("loop");

		float speed = 0.1f;
		if (clipNode->FindAttribute("speed"))
			speed = clipNode->FloatAttribute("speed");

		bool bReverse = false;
		if (clipNode->FindAttribute("reverse"))
			bReverse = clipNode->BoolAttribute("reverse");

		auto clip = std::make_shared<AnimationClip>(
			clipName,
			texturePath,
			static_cast<unsigned int>(std::max(count, 1)),
			Vector2(x, y),
			Vector2(x + w, y + h),
			speed,
			bLoop,
			bReverse
		);

		newClips.push_back(std::move(clip));
		clipNode = clipNode->NextSiblingElement("Clip");
	}

	return newClips;
}
} // namespace AnimationLoader
} // namespace

AnimationClip::AnimationClip(
	std::wstring name,
	std::wstring texturePath,
	unsigned int frameCount,
	Vector2 startPosPixel,
	Vector2 endPosPixel,
	float playRate,
	bool bLoop,
	bool bReverse
)
	: name(std::move(name))
	, texturePath(std::move(texturePath))
	, playRate(playRate)
	, bLoop(bLoop)
	, bReverse(bReverse)
{
	// NOTE: frameCount가 0이면 분할이 불가능하므로 1로 보정한다.
	if (frameCount == 0)
		frameCount = 1;

	texture = TEXTURES.LoadTexture(this->texturePath);
	if (!texture)
		return;

	const Vector2 imageSize = texture->GetSize();

	const Vector2 clipSize(
		std::abs(endPosPixel.x - startPosPixel.x),
		std::abs(endPosPixel.y - startPosPixel.y)
	);

	const Vector2 singleFrameSize(clipSize.x / frameCount, clipSize.y);

	frameSizeUV = Vector2(
		(imageSize.x > 0.0f) ? (singleFrameSize.x / imageSize.x) : 1.0f,
		(imageSize.y > 0.0f) ? (singleFrameSize.y / imageSize.y) : 1.0f
	);

	Vector2 currentPixelPos = startPosPixel;
	keyframes.reserve(frameCount);

	for (unsigned int i = 0; i < frameCount; ++i)
	{
		const Vector2 startUV(
			(imageSize.x > 0.0f) ? (currentPixelPos.x / imageSize.x) : 0.0f,
			(imageSize.y > 0.0f) ? (currentPixelPos.y / imageSize.y) : 0.0f
		);

		keyframes.push_back(startUV);
		currentPixelPos.x += singleFrameSize.x;
	}
}

Animator::Animator()
	: Component("Animator")
{
}

void Animator::Update()
{
	if (!currentClip)
		return;

	// NOTE: 정지 상태에서도 "현재 프레임"은 유지되어야 한다.
	//       재생 중일 때만 프레임 인덱스를 전진시킨다.
	if (bPlaying)
	{
		accumulatedTime += TIME.GetDeltaTime();

		if (accumulatedTime >= currentClip->playRate)
		{
			accumulatedTime -= currentClip->playRate;
			++currentFrameIndex;

			const unsigned int frameCount = static_cast<unsigned int>(currentClip->keyframes.size());
			if (frameCount > 0 && currentFrameIndex >= frameCount)
			{
				if (currentClip->bLoop)
				{
					currentFrameIndex = 0;
				}
				else
				{
					currentFrameIndex = frameCount - 1;
					bPlaying = false;
				}
			}
		}
	}

	ApplyFrameToMaterial();
}

void Animator::ApplyFrameToMaterial()
{
	if (!owner)
		return;

	auto renderer = owner->GetComponent<MeshRenderer>("MeshRenderer");
	if (!renderer)
		return;

	const auto& material = renderer->GetMaterial();
	if (!material)
		return;

	if (currentClip->texture)
	{
		// NOTE: Material이 shared_ptr<Texture>를 받는 구조라면 그대로 전달.
		material->SetTexture(currentClip->texture);
	}

	const auto& frameBuffer = material->GetFrameBuffer();
	if (!frameBuffer)
		return;

	const unsigned int frameCount = static_cast<unsigned int>(currentClip->keyframes.size());
	if (frameCount == 0)
		return;

	unsigned int realFrameIndex = currentFrameIndex;
	if (currentClip->bReverse)
		realFrameIndex = (frameCount - 1) - std::min(currentFrameIndex, frameCount - 1);
	else
		realFrameIndex = std::min(currentFrameIndex, frameCount - 1);

	const Vector2 startUV = currentClip->keyframes[realFrameIndex];
	const Vector2 sizeUV = currentClip->frameSizeUV;

	frameBuffer->SetFrameData(startUV, sizeUV);
}

void Animator::Load(const std::wstring& xmlPath)
{
	const auto& clipsFromFile = AnimationLoader::Load(xmlPath);
	for (const auto& clip : clipsFromFile)
		AddClip(clip);
}

void Animator::AddClip(std::shared_ptr<AnimationClip> clip)
{
	if (!clip)
		return;

	clips[clip->GetName()] = std::move(clip);
}

void Animator::Play(const std::wstring& clipName)
{
	auto it = clips.find(clipName);
	if (it == clips.end())
		return;

	const auto& nextClip = it->second;

	// NOTE: 동일 클립 재생 중이면 무시(정책)
	if (currentClip == nextClip && bPlaying)
		return;

	currentClip = nextClip;
	currentFrameIndex = 0;
	accumulatedTime = 0.0f;
	bPlaying = true;

	// NOTE: 재생 시작 시점에도 즉시 반영
	ApplyFrameToMaterial();
}