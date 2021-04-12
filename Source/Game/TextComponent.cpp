#include "stdafx.h"
#include "TextComponent.h"
#include "tga2d/text/text.h"
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"

TextComponent::TextComponent()
	: 
	myIsActive(false), 
	myPixelPosition()
{
	
}

TextComponent::~TextComponent()
{
	if (myText != nullptr)
	{
		delete myText;
		myText = nullptr;
	}
}

void TextComponent::CreateText(const char* aPathAndName, EFontSize aFontSize, unsigned char aBorderSize)
{
	myText = new Tga2D::CText(aPathAndName, aFontSize, aBorderSize);
}

void TextComponent::Init(Transform& aTransform, GameObject& aGameObject)
{
	myIsActive = true;
	UpdateTransform(aTransform, aGameObject);
}

void TextComponent::Render(Transform& aTransform, GameObject& aGameObject)
{
	if (!myIsActive || myText == nullptr) return;

	UpdateTransform(aTransform, aGameObject);
	myText->Render();
}

void TextComponent::SetText(const std::string& aString)
{
	if (myText == nullptr) return;
	myText->SetText(aString);
}

std::string TextComponent::GetText()
{
	return myText->GetText();
}

void TextComponent::SetColor(Tga2D::CColor aColor)
{
	if (myText == nullptr) return;
	myText->SetColor(aColor);
}

const bool& TextComponent::IsActive() const
{
	return myIsActive;
}

TextComponent& TextComponent::Activate()
{
	myIsActive = true;
	return *this;
}

TextComponent& TextComponent::Deactivate()
{
	myIsActive = false;
	return *this;
}

void TextComponent::SetAnchor(const float anAnchor)
{
	myAnchor = anAnchor;
}

void TextComponent::SetRelativePosition(const float anX, const float aY)
{
	myPixelPosition.x = anX;
	myPixelPosition.y = aY;
}

void TextComponent::SetRelativePositionX(const float anX)
{
	myPixelPosition.x = anX;
}

void TextComponent::SetRelativePositionY(const float aY)
{
	myPixelPosition.y = aY;
}

void TextComponent::SetFontSize(EFontSize aFontSize)
{
	if (myText == nullptr) return;
	myText->SetFontSize(aFontSize);
}

float TextComponent::GetWidth()
{
	return myText->GetWidth();
}

void TextComponent::UpdateTransform(Transform& aTransform, GameObject& aGameObject)
{
	v2f referenceSize = Config::ourReferenceSize;

	v2f renderSize = {};
	renderSize.x = static_cast<float>(Tga2D::CEngine::GetInstance()->GetRenderSize().x);
	renderSize.y = static_cast<float>(Tga2D::CEngine::GetInstance()->GetRenderSize().y);

	v2f percentagePosition = myPixelPosition / referenceSize;

	v2f position = {};
	position.x = renderSize.x * percentagePosition.x;
	position.y = renderSize.y * percentagePosition.y;
	
	position.x = position.x / renderSize.x;
	position.y = position.y / renderSize.y;

	myText->SetPosition(position);
}