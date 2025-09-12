#include "FatLineExample.h"
#include "threepp/utils/GeometryUtils.hpp"
#include "threepp/extras/curves/CatmullRomCurve3.hpp"
FatLineExample::FatLineExample(int width, int height, void* hwnd) 
   : CThreeContainer(width, height,hwnd)
{
	//scene.background = Background(Color(0xEEEEEE));       
}
void FatLineExample::initRenderer() {
    CThreeContainer::initRenderer();
    renderer->setClearColor(Color(0x000000));
    renderer->clear();
    pcamera.farPlane = 1000.0f;
}
void FatLineExample::initCamera() {
    CThreeContainer::initCamera();
    pcamera.fov = 40.0f;
    pcamera.aspect = float(width) / float(height);
    pcamera.nearPlane = 0.1f;
    pcamera.farPlane = 1000.0f;
    pcamera.position.set(-40.0f, 0.0f, 60.0f);
    pcamera.lookAt(Vector3(0, 0, 0));
    pcamera.updateProjectionMatrix();

    camera2.fov = 50.0f;
    camera2.aspect = float(width) / float(height);
    camera->nearPlane = 1.0f;
    camera->farPlane = 1000.0f;
    camera2.position.copy(pcamera.position);
}
void FatLineExample::createObjects(){
    insetWidth = width / 4;
    insetHeight = height / 4;

    std::vector<float> positions;
    std::vector<float> colors;

    auto points = GeometryUtils::Hilbert3D(Vector3(0, 0, 0), 20, 1, 0, 1, 2, 3, 4, 5, 6, 7);

    auto spline = CatmullRomCurve3(points);
    auto divisions = (int) round(12 * points.size());
    Vector3 point;
    Color color;

    for (int i = 0, l = divisions; i < l; i++) {

        auto t = 1.0f * i / l;

        spline.getPoint(t, point);
        positions.insert(positions.end(), {point.x,point.y,point.z});

        color.setHSL(t, 1.0f, 0.5f);
        colors.insert(colors.end(), {color.r, color.g, color.b});
    }


    // Line2 ( LineGeometry, LineMaterial )

    auto geometry = LineGeometry::create();
    geometry->setPositions(positions);
    geometry->setColors(colors);

    matLine = LineMaterial::create();
    matLine->setColor(color.white);
    matLine->setLineWidth(5);// in pixels
    matLine->vertexColors = true;
    matLine->setResolution(Vector2((float) width, (float) height));// to be set by renderer, eventually
    matLine->setDashed(false);
    matLine->setAlphaToCoverage(true);

    
    lineWidth = 5.0f;
    line = Line2::create(geometry, matLine);
    line->computeLineDistances();
    line->scale.set(1, 1, 1);
    line->visible = true;
    scene.add(line);


    // THREE.Line ( THREE.BufferGeometry, THREE.LineBasicMaterial ) - rendered with gl.LINE_STRIP

    auto geo = BufferGeometry::create();
    geo->setAttribute("position", FloatBufferAttribute::create(positions, 3));
    geo->setAttribute("color", FloatBufferAttribute::create(colors, 3));

    matLineBasic = LineBasicMaterial::create({{"color", color.white}, {"vertexColors", true}});
    matLineDashed = LineDashedMaterial::create({{"vertexColors", true}, {"dashSize", 1.0f}, {"gapSize", 1.0f}, {"scale", 2.0f}});

    line1 = Line::create(geo, matLineBasic);
    line1->computeLineDistances();
    line1->visible = false;
    scene.add(line1);
}
void FatLineExample::init()
{
	CThreeContainer::init();

    createObjects();
    setUIControls();
}

void FatLineExample::sizeChanged(int w, int h) {
    CThreeContainer::sizeChanged(w, h);
    insetWidth = w / 4;
    insetHeight = h / 4;
    
    camera2.aspect = float(w) / float(h);
    camera2.updateProjectionMatrix();
}

void FatLineExample::setUIControls() {
    uiPtr = std::make_shared<ImguiFunctionalContext>(hwnd, [&] {
        ImGui::Begin("Fat Lines");
        if (ImGui::Combo("line Type", &lineType, "LineGeometry\0gl.Line")) {
            switch (lineType) {
                case 0:
                    line->visible = true;
                    line1->visible = false;
                    break;
                case 1:
                    line->visible = false;
                    line1->visible = true;
                    break;
            }
        }

        if (ImGui::SliderFloat("width(px)", &lineWidth, 0.0f, 10.0f))
            matLine->setLineWidth(lineWidth);

        if (ImGui::Checkbox("alphaToCoverage", &alphaToCoverage))
            matLine->setAlphaToCoverage(alphaToCoverage);

        if (ImGui::Checkbox("dashed", &dashed)) {
            matLine->setDashed(dashed);
            line1->setMaterial(dashed ? matLineDashed : matLineBasic);
        }

        if (ImGui::SliderFloat("dash scale", &dashScale, 0.5f, 2.0f)) {
            matLine->setDashScale(dashScale);
            matLineDashed->scale = dashScale;
        }

        if (ImGui::Combo("dash/gap", &dashGap, "2 : 1\01 : 1\01 : 2")) {
            switch (dashGap) {
                case 0:
                    matLine->setDashSize(2);
                    matLine->setGapSize(1);
                    matLineDashed->dashSize = 2;
                    matLineDashed->gapSize = 1;
                    break;
                case 1:
                    matLine->setDashSize(1);
                    matLine->setGapSize(1);
                    matLineDashed->dashSize = 1;
                    matLineDashed->gapSize = 1;
                    break;
                case 2:
                    matLine->setDashSize(1);
                    matLine->setGapSize(2);
                    matLineDashed->dashSize = 1;
                    matLineDashed->gapSize = 2;
                    break;
            }
        }
        ImGui::End();
    });
}

void FatLineExample::render() {
    matLine->setResolution(Vector2((float) width, (float) height));
    renderer->setClearColor(0x000000, 0.0f);
    renderer->setViewport(0, 0, width, height);
    renderer->render(scene, *camera);
    if (uiPtr) {
        uiPtr->render();
    }
    renderer->setClearColor(0x222222, 1);

    renderer->clearDepth();// important!

    renderer->setScissorTest(true);

    renderer->setScissor(20, 20, insetWidth, insetHeight);

    renderer->setViewport(20, 20, insetWidth, insetHeight);

    camera2.position.copy(camera->position);
    camera2.quaternion.copy(camera->quaternion);

    // renderer will set this eventually
    matLine->setResolution(Vector2(insetWidth, insetHeight));// resolution of the inset viewport

    renderer->render(scene, camera2);

    renderer->setScissorTest(false);
}

int main() {

    Canvas canvas("FatLine Example");    
    FatLineExample example(canvas.size().width(), canvas.size().height(),(void*)canvas.windowPtr());
    IOCapture capture;
    capture.preventMouseEvent = [&] {
        return ImGui::GetIO().WantCaptureMouse;
    };
    canvas.setIOCapture(&capture);
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
