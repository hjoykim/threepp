#pragma once
#include "ThreeContainer.h"

class CMaterialLightAnimation : public CThreeContainer {
private:
    std::shared_ptr<Mesh> plane, cube, sphere;
    float rotationSpeed = 0.02f;
    float bouncingSpeed = 0.04f;
    float step = 0.0f;
public:
    CMaterialLightAnimation() {}
    CMaterialLightAnimation(int width, int height,void* hwnd) : CThreeContainer(width, height,hwnd)
    {
        scene.background = Background(Color(0xEEEEEE));
    }
    ~CMaterialLightAnimation() = default;
    virtual void init() override;
    virtual void initLighting() override;
    virtual void setUIControls();
    virtual void render() override;
};

