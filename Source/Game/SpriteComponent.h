/*
*	Author: Elia Rönning
*/

#pragma once
#include "Component.hpp"
#include "../External/Headers/CU/Vector2.hpp"
#include "../External/Headers/CU/Vector4.hpp"
#include <string>

class SpritebatchComponent;
struct Transform;
namespace Tga2D
{
    class CSprite;
    class STextureRect;
}

class SpriteComponent : public Component
{
    friend SpritebatchComponent;

public:
    /* Constructors & Destructor */
    SpriteComponent();
    SpriteComponent(const SpriteComponent& aSpriteComponent) = default;
    ~SpriteComponent();

    /* Component Overrides */
    void Init(Transform& aTransform, GameObject& aGameObject) override;

    void UpdateSprite(Tga2D::CSprite* aSprite, const v2f& aPos, const v2f& aSize, const v2f& aPivot, const float& aRot, const v4f& aColor, const v4f& aRect);

    void Render(Transform& aTransform, GameObject& aGameObject) override;

    /* Sprite */
    SpriteComponent& SetSpritePath(const std::string& aSpritePath);
    const std::string& GetSpritePath() const;
    Tga2D::CSprite* GetCSprite();

    /* Alpha */
    const float& GetAlpha() const;
    SpriteComponent& SetAlpha(const float& anAlpha);

    /* Relative Position */
    const v2f& GetRelativePosition() const;
    const float& GetRelativePositionX() const;
    const float& GetRelativePositionY() const;
    SpriteComponent& SetRelativePosition(const v2f& aRelativePosition);
    SpriteComponent& SetRelativePositionX(const float& anX);
    SpriteComponent& SetRelativePositionY(const float& aY);

    /* Relative Rotation */
    const float& GetRelativeRotation() const;
    SpriteComponent& SetRelativeRotation(const float& aRelativeRotation);

    /* Size */
    const v2f& GetSize() const;
    const float& GetSizeX() const;
    const float& GetSizeY() const;
    const v2f GetImageSize() const;
    SpriteComponent& SetSize(const v2f& aRelativeSize);
    SpriteComponent& SetSizeX(const float& anX);
    SpriteComponent& SetSizeY(const float& aY);

    /* Corners */
    v2f GetTopLeft(const Transform& aTransform) const;
    v2f GetTopRight(const Transform& aTransform) const;
    v2f GetBottomLeft(const Transform& aTransform) const;
    v2f GetBottomRight(const Transform& aTransform) const;

    /* Z-Index */
    SpriteComponent& SetZIndex(const int32_t& aZIndex);
    const int32_t& GetZIndex() const;

    /* Color */
    void SetColor(const v4f& aColor);
    const v4f& GetColor() const;

    /* Rect */
    SpriteComponent& SetSpriteRect(const float& aX, const float& aY, const float& aEndX, const float& aEndY);
    SpriteComponent& SetSpriteRectPixel(const uint32_t& anX, const uint32_t& aY, const uint32_t& anEndX, const uint32_t& anEndY);

    /* Sampling & Blend */
    SpriteComponent& SetSamplerState(const ESamplerFilter& aSamplerFilter);
    SpriteComponent& SetBlendState(const EBlendState& aBlendState);
    const EBlendState& GetBlendState() const;

    /* Active */
    const bool& IsActive() const;
    SpriteComponent& Activate();
    SpriteComponent& Deactivate();
    const bool GetShouldRender();

    /* Force Render */
    void SetForceRender(const bool& aIsForced);
    const bool& GetForceRender() const;

private:
    /* Sprite */
    std::string mySpritePath;
    Tga2D::CSprite* mySprite;
    v4f myRect;
    v4f myColor;
    v2ui myImageSize;

    /* Transform */
    v2f myRelativePosition;
    v2f mySize;
    int32_t myZIndex;
    float myRelativeRotation;

    /* Rendering */
    float myAlpha;
    EBlendState myBlendState;
    ESamplerFilter mySamplerFilter;

    /* States */
    bool myIsActive;
    bool myIsPartOfBatch;
    bool myForceRender;
    bool myShouldRender;
};