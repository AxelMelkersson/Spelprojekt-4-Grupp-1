/*
*	Author: Elia R�nning
*/

#include "stdafx.h"
#include "SpriteComponent.h"
#include <tga2d/sprite/sprite.h>
#include "Transform.hpp"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "GameWorld.h"
#include "Game.h"

#include "../External/Headers/CU/Utilities.h"

#include <vector>

/* Constructors & Destructor */
SpriteComponent::SpriteComponent()
	: mySpritePath(std::string())
	, mySprite(nullptr)

	, myForceRender(false)
	, myIsCeilingToPixel(false)
	, myAlpha(1.0f)
	, myZIndex(0)
	, myRelativePosition()
	, myRelativeRotation(0.0f)
	, mySize({ 1.0f, 1.0f })
	, myBlendState(EBlendState::EBlendState_Alphablend)
	, mySamplerFilter(ESamplerFilter_Bilinear)
	, myRect(v4f({ 0.0f, 0.0f, 1.0f, 1.0f }))
	, myColor(v4f(1.0f, 1.0f, 1.0f, 1.0f))
	, myImageSize({ 0U, 0U })

	, myIsActive(true)
	, myIsPartOfBatch(false)
{}

SpriteComponent::~SpriteComponent()
{
	if (mySprite != nullptr)
	{
		delete mySprite;
		mySprite = nullptr;
	}
}

/* Component Overrides */
void SpriteComponent::Init(Transform & aTransform, GameObject& aGameObject)
{
	aTransform;
	aGameObject;

	if (myIsPartOfBatch) return;
	mySprite = new Tga2D::CSprite(mySpritePath.c_str());
	mySprite->SetBlendState(myBlendState);
}

void SpriteComponent::UpdateSprite(Tga2D::CSprite* aSprite, const v2f& aPos, const v2f& aSize, const v2f& aPivot, const float& aRot, const v4f& aColor, const v4f& aRect)
{
	aSprite->SetPosition(aPos);
	aSprite->SetSizeRelativeToScreen(aSize);
	aSprite->SetPivot(aPivot);
	aSprite->SetRotation(aRot);
	aSprite->SetColor(Tga2D::CColor(aColor.x, aColor.y, aColor.z, aColor.w));

	aSprite->SetTextureRect(aRect.x, aRect.y, aRect.z, aRect.w);
}


void SpriteComponent::Render(Transform & aTransform, GameObject& aGameObject)
{
	if (mySprite == nullptr || !myIsActive)
	{
		return;
	}

	v2f offset = { 0.0f, 0.0f };
	float height = static_cast<float>(CGameWorld::GetInstance()->Game()->GetZoomY());
	float width = static_cast<float>(CGameWorld::GetInstance()->Game()->GetZoomX());

	constexpr float aspectRatioX = 16.0f;
	constexpr float aspectRatioY = 9.0f;
	constexpr float aspectRatio = aspectRatioX / aspectRatioY;
	constexpr float inversedAspectRatio = aspectRatioY / aspectRatioX;

	if (width / aspectRatioX < height / aspectRatioY)
	{
		height = width * inversedAspectRatio;
	}
	else
	{
		width = height * aspectRatio;
	}

	float alpha = myAlpha;
	float zoom = 1.0f;

	Scene* scene = aGameObject.GetScene();
	if (scene != nullptr)
	{
		Camera& camera = scene->GetCamera();
		zoom = camera.GetZoom();

		v2f cameraPosition = camera.GetPosition();
		cameraPosition.x = ceil(cameraPosition.x);
		cameraPosition.y = ceil(cameraPosition.y);

		v2f trueSize = v2f(Utils::Abs(mySize.x), mySize.y);

		v2f spriteMin = GetTopLeft(aTransform);
		v2f spriteMax = GetBottomRight(aTransform);

		v2f cameraMin = cameraPosition - trueSize;
		v2f cameraMax = v2f(cameraPosition.x + (width / zoom), cameraPosition.y + (height / zoom)) + trueSize;

		if (!(spriteMin.x <= cameraMax.x && spriteMax.x >= cameraMin.x &&
			spriteMin.y <= cameraMax.y && spriteMax.y >= cameraMin.y) && !myForceRender)
		{
			myShouldRender = false;
			mySprite->SetShouldRender(false);
			return;
		}
		else
		{
			myShouldRender = true;
			mySprite->SetShouldRender(true);
		}


		offset = cameraPosition * -1.0f;

		alpha *= camera.GetAlpha();

		v2f truePosition = v2f(aTransform.myPosition.x + myRelativePosition.x + offset.x, aTransform.myPosition.y + myRelativePosition.y + offset.y);

		
		truePosition.x = ceil(truePosition.x);
		truePosition.y = ceil(truePosition.y);
		

		const v2f position = v2f(truePosition.x / width * zoom, truePosition.y / height * zoom);
		const v2f size = v2f((mySize.x / height) * zoom, ((mySize.y) / height) * zoom);
		const v4f color = v4f(myColor.x, myColor.y, myColor.z, myColor.w * alpha);
		UpdateSprite(mySprite, position, size, aTransform.myPivot, aTransform.myRotation + myRelativeRotation, myColor, myRect);
	}

	if (myIsPartOfBatch)
	{
		return;
	}

	mySprite->Render();
}

/* Sprite */
SpriteComponent& SpriteComponent::SetSpritePath(const std::string& aSpritePath)
{
	mySpritePath = aSpritePath;
	delete mySprite;
	mySprite = nullptr;
	mySprite = new Tga2D::CSprite(mySpritePath.c_str());
	mySprite->SetBlendState(myBlendState);
	myImageSize = mySprite->GetImageSize();
	mySize = GetImageSize();

	return *this;
}
const std::string& SpriteComponent::GetSpritePath() const
{
	return mySpritePath;
}

Tga2D::CSprite* SpriteComponent::GetCSprite()
{
	return mySprite;
}

/* Alpha */
const float& SpriteComponent::GetAlpha() const
{
	return myAlpha;
}
SpriteComponent& SpriteComponent::SetAlpha(const float& anAlpha)
{
	myAlpha = anAlpha;

	return *this;
}

/* Relative Position*/
const v2f& SpriteComponent::GetRelativePosition() const
{
	return myRelativePosition;
}
const float& SpriteComponent::GetRelativePositionX() const
{
	return myRelativePosition.x;
}
const float& SpriteComponent::GetRelativePositionY() const
{
	return myRelativePosition.y;
}
SpriteComponent& SpriteComponent::SetRelativePosition(const v2f& aRelativePosition)
{
	myRelativePosition = aRelativePosition;

	return *this;
}
SpriteComponent& SpriteComponent::SetRelativePositionX(const float& anX)
{
	myRelativePosition.x = anX;

	return *this;
}
SpriteComponent& SpriteComponent::SetRelativePositionY(const float& aY)
{
	myRelativePosition.y = aY;

	return *this;
}

/* Relative Rotation */
const float& SpriteComponent::GetRelativeRotation() const
{
	return myRelativeRotation;
}
SpriteComponent& SpriteComponent::SetRelativeRotation(const float& aRelativeRotation)
{
	myRelativeRotation = aRelativeRotation;

	return *this;
}

/* Size */
const v2f& SpriteComponent::GetSize() const
{
	return mySize;
}
const float& SpriteComponent::GetSizeX() const
{
	return mySize.x;
}
const float& SpriteComponent::GetSizeY() const
{
	return mySize.y;
}
const v2f SpriteComponent::GetImageSize() const
{
	return { static_cast<float>(myImageSize.x), static_cast<float>(myImageSize.y) };
}
SpriteComponent& SpriteComponent::SetSize(const v2f& aRelativeSize)
{
	mySize = aRelativeSize;

	return *this;
}
SpriteComponent& SpriteComponent::SetSizeX(const float& anX)
{
	mySize.x = anX;

	return *this;
}
SpriteComponent& SpriteComponent::SetSizeY(const float& aY)
{
	mySize.y = aY;

	return *this;
}

/* Corners */
v2f SpriteComponent::GetTopLeft(const Transform& aTransform) const
{
	const float x = aTransform.myPosition.x + myRelativePosition.x - (mySize.x * aTransform.myPivot.x);
	const float y = aTransform.myPosition.y + myRelativePosition.y - (mySize.y * aTransform.myPivot.y);
	return { x, y };
}
v2f SpriteComponent::GetTopRight(const Transform& aTransform) const
{
	const float x = aTransform.myPosition.x + myRelativePosition.x + (mySize.x * (1.0f - aTransform.myPivot.x));
	const float y = aTransform.myPosition.y + myRelativePosition.y - (mySize.y * aTransform.myPivot.y);
	return { x, y };
}
v2f SpriteComponent::GetBottomLeft(const Transform& aTransform) const
{
	const float x = aTransform.myPosition.x + myRelativePosition.x - (mySize.x * aTransform.myPivot.x);
	const float y = aTransform.myPosition.y + myRelativePosition.y + (mySize.y * (1.0f - aTransform.myPivot.y));
	return { x, y };
}
v2f SpriteComponent::GetBottomRight(const Transform& aTransform) const
{
	const float x = aTransform.myPosition.x + myRelativePosition.x + (mySize.x * (1.0f - aTransform.myPivot.x));
	const float y = aTransform.myPosition.y + myRelativePosition.y + (mySize.y * (1.0f - aTransform.myPivot.y));
	return { x, y };
}

/* Z-Index */
SpriteComponent& SpriteComponent::SetZIndex(const int32_t& aZIndex)
{
	myZIndex = aZIndex;

	return *this;
}
const int32_t& SpriteComponent::GetZIndex() const
{
	return myZIndex;
}

/* Color */
void SpriteComponent::SetColor(const v4f& aColor)
{
	myColor = aColor;
}
const v4f& SpriteComponent::GetColor() const
{
	return myColor;
}

/* Rect */
SpriteComponent& SpriteComponent::SetSpriteRect(const float& aX, const float& aY, const float& aEndX, const float& aEndY)
{
	myRect = { aX, aY, aEndX, aEndY };

	return *this;
}

SpriteComponent& SpriteComponent::SetSpriteRectPixel(const uint32_t& anX, const uint32_t& aY, const uint32_t& anEndX, const uint32_t& anEndY)
{
	if (myImageSize.x > 0U && myImageSize.y > 0U)
	{
		myRect = { static_cast<float>(anX) / myImageSize.x, static_cast<float>(aY) / myImageSize.y, static_cast<float>(anEndX) / myImageSize.x, static_cast<float>(anEndY) / myImageSize.y };
	}

	return *this;
}

/* Sampling & Blend */
SpriteComponent& SpriteComponent::SetSamplerState(const ESamplerFilter& aSamplerFilter)
{
	//mySprite->SetSamplerState(aSamplerFilter, ESamplerAddressMode_Clamp);
	mySamplerFilter = aSamplerFilter;

	return *this;
}
SpriteComponent& SpriteComponent::SetBlendState(const EBlendState& aBlendState)
{
	myBlendState = aBlendState;

	return *this;
}
const EBlendState& SpriteComponent::GetBlendState() const
{
	return myBlendState;
}

/* Active */
const bool& SpriteComponent::IsActive() const
{
	return myIsActive;
}
SpriteComponent& SpriteComponent::Activate()
{
	myIsActive = true;

	return *this;
}
SpriteComponent& SpriteComponent::Deactivate()
{
	myIsActive = false;

	return *this;
}

const bool SpriteComponent::GetShouldRender()
{
	return myShouldRender;
}

/* Force Render*/
void SpriteComponent::SetForceRender(const bool& aIsForced)
{
	myForceRender = aIsForced;
}
const bool& SpriteComponent::GetForceRender() const
{
	return myForceRender;
}

void SpriteComponent::SetCeilPosition(const bool& aIsCeiled)
{
	myIsCeilingToPixel = aIsCeiled;
}