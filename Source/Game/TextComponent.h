#pragma once
#include <string>
#include <tga2d/text/text.h>
#include "Component.hpp"
#include "EFontsSizes.hpp"

class TextComponent : public Component
{
public:

    TextComponent();
    TextComponent(const TextComponent& aTextComponent) = default;
    ~TextComponent();

    void CreateText(const char* aPathAndName, EFontSize aFontSize, unsigned char aBorderSize);

    void Init(Transform& aTransform, GameObject& aGameObject) override;
    void Render(Transform& aTransform, GameObject& aGameObject) override;

    void SetText(const std::string aString);
    std::string GetText();
    void SetColor(Tga2D::CColor aColor);
    void SetAnchor(const float anAnchor);
    void SetRelativePosition(const float anX, const float aY);
    void SetRelativePositionX(const float anX);
    void SetRelativePositionY(const float aY);
    void SetFontSize(EFontSize aFontSize);

    float GetWidth();

    const bool& IsActive() const;
    TextComponent& Activate();
    TextComponent& Deactivate();

private:
    void UpdateTransform(Transform& aTransform, GameObject& aGameObject);

    Tga2D::CText* myText;
    Tga2D::Vector2f myPixelPosition;
    float myAnchor = 0.0f;
    bool myIsActive;
};