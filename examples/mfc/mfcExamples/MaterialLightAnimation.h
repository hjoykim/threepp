#pragma once
#include "ThreeContainer.h"
class CMaterialLightAnimation : public CThreeContainer {
private:
    std::shared_ptr<Mesh> plane, cube, sphere;
    float rotationSpeed = 0.02f;
    float bouncingSpeed = 0.04f;
    float step = 0.0f;
public:
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
inline void CMaterialLightAnimation::init()
{
    CThreeContainer::init();
    auto planeGeometry = PlaneGeometry::create(60, 20);
    auto planeMaterial = MeshLambertMaterial::create();
    planeMaterial->color = Color().setHex(0xcccccc);
    plane = Mesh::create(planeGeometry, planeMaterial);
    plane->receiveShadow = true;
    plane->rotation.x =(float)(-0.5f * math::PI);

    plane->position.set(15.0f, 0.0f, 0.0f);

    auto cubeGeometry = BoxGeometry::create(4, 4, 4);
    auto cubeMaterial = MeshLambertMaterial::create();
    cubeMaterial->color = Color().setHex(0xff0000);
    cubeMaterial->wireframe = false;
    cube = Mesh::create(cubeGeometry, cubeMaterial);
    cube->position.set(-4.0f, 3.0f, 0.0f);
    cube->castShadow = true;



    auto sphereGeometry = SphereGeometry::create(4, 20, 20);
    auto sphereMaterial = MeshLambertMaterial::create();
    sphere = Mesh::create(sphereGeometry, sphereMaterial);
    sphereMaterial->color = Color(0x7777ff);
    sphereMaterial->wireframe = false;
    sphere->castShadow = true;

    sphere->position.set(20, 4, 2);

    scene.add(cube);
    scene.add(sphere);
    scene.add(plane);

    setUIControls();
}

inline void CMaterialLightAnimation::initLighting()
{
    auto spotLight = SpotLight::create(Color(0xffffff));
    spotLight->position.set(-40, 40, -15);
    spotLight->castShadow = true;
    //spotLight->shadow->mapSize = Vector2(512, 512);
    //spotLight->shadow->camera->_far = 130;
    //spotLight->shadow->camera->_near = 40;

    scene.add(spotLight);

    auto ambientLight = AmbientLight::create(Color(0x353535));
    scene.add(ambientLight);
}

inline void CMaterialLightAnimation::setUIControls() 
{    
    uiPtr = std::make_shared<ImguiFunctionalContext>(hwnd, [&] {
            ImGui::Begin("MaterialLightAnimation");
            ImGui::Text("This is Rotaion Speed Control box");
            ImGui::SliderFloat("RotationSpeed", &rotationSpeed, 0.0f, 0.5f);
            ImGui::SliderFloat("BouncingSpeed", &bouncingSpeed, 0.0f, 0.5f);
            ImGui::End();
    });
    capture.preventMouseEvent = [] {
        return ImGui::GetIO().WantCaptureMouse;
    };
    this->setIOCapture(&capture);
}

inline void CMaterialLightAnimation::render()
{
    cube->rotation.x = cube->rotation.x + rotationSpeed;
    cube->rotation.y =cube->rotation.y + rotationSpeed;
    cube->rotation.z = cube->rotation.z + rotationSpeed;

    step += bouncingSpeed;

    sphere->position.x = 20 + (10 * cos(step));
    sphere->position.y = 2 + (10 * abs(sin(step)));

    CThreeContainer::render();
}


