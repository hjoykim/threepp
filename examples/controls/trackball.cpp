#include "threepp/controls/TrackballControls.hpp"
#include "threepp/threepp.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include <iostream>

using namespace threepp;

int main() {
    Canvas canvas("Trackball controls");
    GLRenderer renderer(canvas.size());
    Camera* camera;
    PerspectiveCamera perspectiveCamera = PerspectiveCamera(60, canvas.aspect(), 1, 1000);
    perspectiveCamera.position.z = 500;
    unsigned frustumSize = 400;
    OrthographicCamera orthographicCamera = OrthographicCamera(frustumSize * canvas.aspect() / -2, frustumSize * canvas.aspect() / 2, frustumSize / 2, frustumSize / -2, 1, 1000);
    orthographicCamera.position.z = 500;

    Scene scene;
    scene.background = Color(0xf0f0f0);
    scene.fog = FogExp2(0xcccccc, 0.002f);

    auto geometry = CylinderGeometry::create(0, 10, 30, 4, 1);
    auto material = MeshPhongMaterial::create({{" color", 0xffffff}, {"flatShading", true}});

    for (auto i = 0; i < 500; i++) {

        auto mesh = Mesh::create(geometry, material);
        mesh->position.x = (math::randFloat() - 0.5f) * 1000;
        mesh->position.y = (math::randFloat() - 0.5f) * 1000;
        mesh->position.z = (math::randFloat() - 0.5f) * 1000;
        mesh->updateMatrix();
        mesh->matrixAutoUpdate = false;
        scene.add(mesh);
    }

    // lights

    auto dirLight1 = DirectionalLight::create(0xffffff);
    dirLight1->position.set(1, 1, 1);
    scene.add(dirLight1);

    auto dirLight2 = DirectionalLight::create(0x002288);
    dirLight2->position.set(-1, -1, -1);
    scene.add(dirLight2);

    auto ambientLight = AmbientLight::create(0x222222);
    scene.add(ambientLight);
    Vector4 screen = Vector4(0, 0, canvas.size().width, canvas.size().height);
    auto controls = TrackballControls(perspectiveCamera, screen, canvas);
    controls.rotateSpeed = 1.0f;
    controls.zoomSpeed = 1.2f;
    controls.panSpeed = 0.8f;
    camera = dynamic_cast<Camera*>(&perspectiveCamera);

    canvas.onWindowResize([&](WindowSize size) {
        renderer.setSize(size);
        screen.z = size.width;
        screen.w = size.height;
        perspectiveCamera.aspect = size.aspect();
        perspectiveCamera.updateProjectionMatrix();

        orthographicCamera.left = -(frustumSize * size.aspect() / 2);
        orthographicCamera.right = frustumSize * size.aspect() / 2;
        orthographicCamera.top = 0.5f*frustumSize;
        orthographicCamera.bottom = -0.5f*frustumSize;
        orthographicCamera.updateProjectionMatrix();
        controls.sizeChanged(screen);



    });
    bool useOrthographic{false};

    ImguiFunctionalContext ui(canvas.windowPtr(), [&] {
        //ImGui::SetNextWindowPos({0, 0}, 0, {0, 0});
        //ImGui::SetNextWindowSize({230, 0}, 0);
        ImGui::Begin("Options");
        if (ImGui::Checkbox("use orthographic", &useOrthographic)) {
            if (!useOrthographic) {
                camera = dynamic_cast<Camera*>(&perspectiveCamera);
                controls.initTrackballControls(perspectiveCamera, screen);
            } else {
                camera = dynamic_cast<Camera*>(&orthographicCamera);
                controls.initTrackballControls(orthographicCamera, screen);
               
            }
            
        }
        ImGui::End();
    });
    
    IOCapture capture{};
    capture.preventMouseEvent = [] {
        return ImGui::GetIO().WantCaptureMouse;
    };

    canvas.setIOCapture(&capture);
    canvas.animate([&] {
        controls.update();
        renderer.render(scene, *camera);
        ui.render();
    });
}