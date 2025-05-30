#pragma once
#include "MaterialLight.h"

 void CMaterialLight::init()
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

 void CMaterialLight::initLighting()
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
#ifndef _MSC_VER
 int main() {

     Canvas canvas("MaterialLight");
     CMaterialLight example(canvas.size().width(), canvas.size().height(), (void*)canvas.windowPtr());


     example.init();
     example.controls = std::make_shared<OrbitControls>(example.pcamera, canvas);
     canvas.onWindowResize([&](WindowSize size) {
         example.pcamera.aspect = size.aspect();
         example.pcamera.updateProjectionMatrix();

         example.sizeChanged(size.width(), size.height());
         });
     canvas.animate([&] {
         example.render();
         });

 }
#endif
