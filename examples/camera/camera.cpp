#include "threepp/threepp.hpp"
#include "threepp/cameras/OrthographicCamera.hpp"
#include "threepp/cameras/PerspectiveCamera.hpp"
#include "threepp/helpers/CameraHelper.hpp"
#include "threepp/math/MathUtils.hpp"
#include <iostream>
using namespace threepp;

const unsigned frustumSize = 600;

Camera* activeCamera;
int main() {
    Canvas canvas("Camera");
    GLRenderer renderer(canvas.size());
    renderer.autoClear = false;
    PerspectiveCamera camera = PerspectiveCamera(50, 0.5 *canvas.aspect(), 1, 10000);
    camera.position.z = 2500;

    PerspectiveCamera cameraPerspective = PerspectiveCamera(50, 0.5 * canvas.aspect(), 150, 1000);
    cameraPerspective.updateProjectionMatrix();

    Scene scene;

    //
    OrthographicCamera cameraOrtho = OrthographicCamera(0.5f * frustumSize * canvas.aspect()  / -2, 0.5f * frustumSize * canvas.aspect()  / 2, frustumSize / 2, frustumSize / -2, 150, 1000);
    cameraOrtho.updateProjectionMatrix();
    std::shared_ptr<CameraHelper> cameraPerspectiveHelper = CameraHelper::create(cameraPerspective);
    scene.add(cameraPerspectiveHelper);

    std::shared_ptr<CameraHelper> cameraOrthoHelper = CameraHelper::create(cameraOrtho);
    scene.add(cameraOrthoHelper);
    //

    activeCamera = static_cast<Camera*>(&cameraPerspective);
    std::shared_ptr<CameraHelper> activeHelper = cameraPerspectiveHelper;


    // counteract different front orientation of cameras vs rig

    cameraOrtho.rotation.y = math::PI;
    cameraPerspective.rotation.y = math::PI;

    Group cameraRig;

    cameraRig.add(cameraPerspective);
    cameraRig.add(cameraOrtho);

    scene.add(cameraRig);

    //

    auto mesh = Mesh::create(
            SphereGeometry::create(100, 16, 8),
            MeshBasicMaterial::create({{"color", 0xffffff}, {"wireframe", true}}));
    scene.add(mesh);

    auto mesh2 = Mesh::create(
            SphereGeometry::create(50, 16, 8),
            MeshBasicMaterial::create({{"color", 0x00ff00}, {"wireframe", true}}));
    mesh2->position.y = 150;
    mesh->add(mesh2);

    auto mesh3 = Mesh::create(
            SphereGeometry::create(5, 16, 8),
            MeshBasicMaterial::create({{"color", 0x0000ff}, {"wireframe", true}}));
    mesh3->position.z = 150;
    cameraRig.add(mesh3);

    //

    auto geometry = BufferGeometry::create();
    std::vector<float> vertices;;

    for (auto i = 0; i < 10000; i++) {

        vertices.push_back(math::randFloatSpread(2000));// x
        vertices.push_back(math::randFloatSpread(2000));// y
        vertices.push_back(math::randFloatSpread(2000));// z
    }

    geometry->setAttribute("position", FloatBufferAttribute::create(vertices, 3));

    auto particles = Points::create(geometry, PointsMaterial::create({{"color", 0x888888}}));
    scene.add(particles);

    canvas.onWindowResize([&](WindowSize size) {
        renderer.setSize(size);

        camera.aspect = 0.5f * size.aspect();
        camera.updateProjectionMatrix();

        cameraPerspective.aspect = 0.5f * size.aspect();
        cameraPerspective.updateProjectionMatrix();

        cameraOrtho.left = -0.5f * frustumSize * size.aspect() / 2.f;
        cameraOrtho.right = 0.5f * frustumSize * size.aspect()  / 2.f;
        cameraOrtho.top = frustumSize / 2.f;
        cameraOrtho.bottom = -(frustumSize / 2.f);
        cameraOrtho.updateProjectionMatrix();
        });
    
    KeyAdapter keyAdapter(KeyAdapter::Mode::KEY_PRESSED, [&](KeyEvent evt) {
        switch (evt.key) {

            case Key::O :/*O*/

                activeCamera = static_cast<Camera*>(&cameraOrtho);
                activeHelper = cameraOrthoHelper;
                activeCamera->updateProjectionMatrix();
                break;

            case Key::P: /*P*/

                activeCamera = static_cast<Camera*>(&cameraPerspective);
                activeHelper = cameraPerspectiveHelper;
                
                break;
        }

    });
    canvas.addKeyListener(keyAdapter);

    Clock clock;
    float r = 0;
    canvas.animate([&] {
        r +=0.005f;

        mesh->position.x = 700 * std::cos(r);
        mesh->position.z = 700 * std::sin(r);
        mesh->position.y = 700 * std::sin(r);

        mesh->children[0]->position.x = 70 * std::cos(2 * r);
        mesh->children[0]->position.z = 70 * std::sin(r);

        if (activeCamera == &cameraPerspective) {

            cameraPerspective.fov = 35 + 30 * std::sin(0.5f * r);
            cameraPerspective.far = mesh->position.length();
            cameraPerspective.updateProjectionMatrix();

            cameraPerspectiveHelper->update();
            cameraPerspectiveHelper->visible = true;

            cameraOrthoHelper->visible = false;

        } else {

            cameraOrtho.far = mesh->position.length();
            cameraOrtho.updateProjectionMatrix();

            cameraOrthoHelper->update();
            cameraOrthoHelper->visible = true;

            cameraPerspectiveHelper->visible = false;
        }

        cameraRig.lookAt(mesh->position);

        renderer.clear();

        activeHelper->visible = false;

        renderer.setViewport(0, 0, canvas.size().width / 2, canvas.size().height);
        renderer.render(scene, *activeCamera);

        activeHelper->visible = true;

        renderer.setViewport(canvas.size().width / 2, 0, canvas.size().width / 2, canvas.size().height);
        renderer.render(scene,camera);

    });
}