#pragma once
#include "ThreeContainer.h"
class CFirstSceneExample : public CThreeContainer {
public:
	CFirstSceneExample(int width,int height) : CThreeContainer(width, height) 
	{
		scene.background = Background(Color(0xEEEEEE));       
	}
	~CFirstSceneExample() = default;
	virtual void init() override;
};
inline void CFirstSceneExample::init()
{
	CThreeContainer::init();
	auto planeGeometry = PlaneGeometry::create(60, 20);
    auto planeMaterial = MeshBasicMaterial::create();
    planeMaterial->color = Color(0xcccccc);
    auto plane = Mesh::create(planeGeometry, planeMaterial);

    plane->rotation.x =(float)(-0.5f * math::PI);

    plane->position.set(15.0f, 0.0f, 0.0f);

    auto cubeGeometry = BoxGeometry::create(4, 4, 4);
    auto cubeMaterial = MeshBasicMaterial::create();
    cubeMaterial->color = Color(0xff0000);
    cubeMaterial->wireframe = true;
    auto cube = Mesh::create(cubeGeometry, cubeMaterial);
    cube->position.set(-4.0f, 3.0f, 0.0f);
    //scene->add(axis);
    scene.add(plane);
    scene.add(cube);

    auto sphereGeometry = SphereGeometry::create(4, 20, 20);
    auto sphereMaterial = MeshBasicMaterial::create();
    auto sphere = Mesh::create(sphereGeometry, sphereMaterial);
    sphereMaterial->color = Color(0x7777ff);
    sphereMaterial->wireframe = true;

    sphere->position.set(20, 4, 2);
    scene.add(sphere);
}