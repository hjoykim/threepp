#include "threepp/controls/TrackballControls.hpp"
#include "threepp/input/PeripheralsEventSource.hpp"
#include "threepp/cameras/Camera.hpp"
#include "threepp/cameras/PerspectiveCamera.hpp"
#include "threepp/cameras/OrthographicCamera.hpp"
#include "threepp/math/Vector3.hpp"
#include "threepp/math/Vector4.hpp"
#include <iostream>
using namespace threepp;
namespace {
    const float SQRT1_2 = 0.7071067811865476f;
    enum class STATE : int {
        NONE = -1,
        ROTATE = 0,
        ZOOM = 1,
        PAN = 2,
        TOUCH_ROTATE = 3,
        TOUCH_ZOOM_PAN = 4
    };
    const float EPS = 0.000001f;
} // namespace

struct TrackballControls::Impl : public MouseListener,public KeyListener {
    TrackballControls* scope;
    PeripheralsEventSource* canvas;
    Camera* camera;
    unsigned char keys[3] = {65, 83, 68};
    // internal variables
    Vector3 target;

    Vector3 lastPosition;

    float lastZoom = 1;

    STATE state = STATE::NONE;

    STATE keyState = STATE::NONE;

    Vector3 eye;

    Vector3 rotateStart;

    Vector3 rotateEnd;

    Vector2 zoomStart;

    Vector2 zoomEnd;

    float touchZoomDistanceStart = 0.0f;

    float touchZoomDistanceEnd = 0.0f;

    Vector2 panStart;

    Vector2 panEnd;

    Vector4 screen;

    Vector3 target0;

    Vector3 position0;

    Vector3 up0;

    float zoom0;

    Impl(TrackballControls& scope, Camera& camera, Vector4& screen, PeripheralsEventSource& canvas)
        : scope(&scope), camera(&camera), screen(screen), canvas(&canvas)
    {
        target.set(0, 0, 0);
        lastPosition.set(0, 0, 0);
        eye.set(0, 0, 0);
        rotateStart.set(0, 0, 0);
        rotateEnd.set(0, 0, 0);
        zoomStart.set(0, 0);
        zoomEnd.set(0, 0);
        panStart.set(0, 0);
        panEnd.set(0, 0);

        target0.copy(target);
        position0.copy(camera.position);
        up0.copy(camera.up);
        zoom0 = camera.zoom;

        activate();

    }
    void activate() 
    {
        canvas->addMouseListener(*this);
    }
    bool isPerspectiveCamera()
    {
        if (dynamic_cast<PerspectiveCamera*>(camera) != nullptr) return true;
        else
            return false;
    }
    bool isOrthographicCamera() {
        if (dynamic_cast<OrthographicCamera*>(camera) != nullptr) return true;
        else
            return false;
    }
    void deactivate() 
    {
        canvas->removeMouseListener(*this);
    }

    void initTrackballControls(Camera& camera, Vector4& screen) {

        target.set(0, 0, 0);
        lastPosition.set(0, 0, 0);
        eye.set(0, 0, 0);
        rotateStart.set(0, 0, 0);
        rotateEnd.set(0, 0, 0);
        zoomStart.set(0, 0);
        zoomEnd.set(0, 0);
        panStart.set(0, 0);
        panEnd.set(0, 0);

        this->camera = &camera;
        this->screen.copy(screen);
        target0.copy(target);
        position0.copy(this->camera->position);
        up0.copy(this->camera->up);
        zoom0 = this->camera->zoom;
    }
    Vector2 getMouseOnScreen(float pageX, float pageY)
    {
        return Vector2((pageX-screen.x)/screen.z, (pageY-screen.y)/screen.w);
    }

    Vector3 getMouseProjectionOnBall(float pageX, float pageY)
    {
        // TODO: 여기에 return 문을 삽입합니다.
        Vector3 mouseOnBall = Vector3(
                (pageX - screen.z * 0.5f - screen.x ) / (screen.z * 0.5f),
                (screen.w * 0.5f + screen.y - pageY) / (screen.w * 0.5f),
                0.0f);

        float length = mouseOnBall.length();

        if (scope->noRoll) {
            if (length < SQRT1_2)
                mouseOnBall.z = (float) sqrt(1.0f - length * length);
            else
                mouseOnBall.z = 0.5f / length;
        } else if (length > 1.0f)
            mouseOnBall.normalize();
        else
            mouseOnBall.z = (float) sqrt(1.0f - length * length);

        Vector3 camPos = camera->position;
        eye = camPos - target;
        Vector3 upClone;
        upClone.copy(camera->up);

        Vector3 projection;
        upClone.normalize();

        projection = upClone.multiplyScalar(mouseOnBall.y);

        Vector3 cross;
        cross.copy(camera->up);
        cross.cross(eye);
        cross.normalize();
        cross.multiplyScalar(mouseOnBall.x);

        projection = projection.add(cross);

        //  projection.add(_eye.normalize().scale(mouseOnBall.z));
        Vector3 eyeClone;
        eyeClone.copy(eye);
        eyeClone.normalize();

        projection.add(eyeClone.multiplyScalar(mouseOnBall.z));

        return projection;
    }

    void rotateCamera()
    {
        Vector3 axis;
        Quaternion quaternion;

        float angle = (float) acos(rotateStart.dot(rotateEnd) / rotateStart.length() / rotateEnd.length());

        if (angle > 0) {
            axis.crossVectors(rotateStart, rotateEnd).normalize();

            angle *= scope->rotateSpeed;

            quaternion.setFromAxisAngle(axis, -angle);

            eye.applyQuaternion(quaternion);
            camera->up.applyQuaternion(quaternion);

            rotateEnd.applyQuaternion(quaternion);

            if (scope->staticMoving) {
                rotateStart.copy(rotateEnd);
            } else {
                quaternion.setFromAxisAngle(axis, angle * (scope->dynamicDampingFactor - 1.0f));
                rotateStart.applyQuaternion(quaternion);
            }
        }
    }

    void zoomCamera() 
    {
        if (state == STATE::TOUCH_ZOOM_PAN) {
            float factor = touchZoomDistanceStart / touchZoomDistanceEnd;
            touchZoomDistanceStart = touchZoomDistanceEnd;
            if (isPerspectiveCamera())
                eye = eye * factor;
            else if (isOrthographicCamera()) {
                camera->zoom *= factor;
                camera->updateProjectionMatrix();
            }
        } else {
            float factor = (float) (1.0f + (zoomEnd.y - zoomStart.y) * scope->zoomSpeed);
            if (factor != 1.0f && factor > 0.0f) {
                if (isPerspectiveCamera())
                    eye.multiplyScalar(factor);
                else if (isOrthographicCamera()) {
                    camera->zoom /= factor;
                    camera->updateProjectionMatrix();
                }
            }
            if (scope->staticMoving) {
                zoomStart.set(zoomEnd.x, zoomEnd.y);
            } else {
                zoomStart.y += (zoomEnd.y - zoomStart.y) * scope->dynamicDampingFactor;
            }
        }

    }

    void panCamera() 
    {
        Vector2 mouseChange;
        Vector3 objectUp;
        Vector3 pan;

        mouseChange.copy(panEnd);
        mouseChange.sub(panStart);

        if (mouseChange.lengthSq() > 0) {
            if (isOrthographicCamera()) {
                OrthographicCamera* ocamera = dynamic_cast<OrthographicCamera*>(camera);
                float scale_x = (ocamera->right - ocamera->left) / ocamera->zoom / screen.z;
                float scale_y = (ocamera->top - ocamera->bottom) / ocamera->zoom / screen.z;

                mouseChange.x *= scale_x;
                mouseChange.y *= scale_y;
            }
            mouseChange.multiplyScalar(eye.length() * scope->panSpeed);
            pan.copy(eye);
            pan.cross(camera->up);
            pan.setLength(mouseChange.x);
            pan.add(objectUp.copy(camera->up).setLength(mouseChange.y));

            camera->position.add(pan);
            target.add(pan);


            if (scope->staticMoving) {
                panStart.copy(panEnd);
            } else {                
                panStart.add(mouseChange.subVectors(panEnd, panStart).multiplyScalar(scope->dynamicDampingFactor));
            }
        }

    }

    void checkDistance() 
    {
        if (!scope->noZoom || !scope->noPan) {

            if (eye.lengthSq() > scope->maxDistance * scope->maxDistance) {
                eye.normalize();
                eye.multiplyScalar(scope->maxDistance);

                camera->position = target + eye;
            }

            if (eye.lengthSq() < scope->minDistance * scope->minDistance) {
                eye.normalize();

                eye.multiplyScalar(scope->minDistance);

                camera->position = target + eye;
            }
        }
    }

    void update() 
    {
        eye.subVectors(camera->position, target);
        if (!scope->noRotate) {
            rotateCamera();
        }

        if (!scope->noZoom) {
            zoomCamera();
        }

        if (!scope->noPan) {
            panCamera();
        }

        // object.position =  target + _eye;
        camera->position = target + eye;


        if (isPerspectiveCamera()) {
            checkDistance();

            // object.lookAt( target );
            camera->lookAt(target);

            // distanceToSquared
            if ((lastPosition - camera->position).lengthSq() > EPS) {
                //
                //   dispatchEvent( changeEvent );

                lastPosition.copy(camera->position);
            }
        } 
        else if (isOrthographicCamera()) {
            camera->lookAt(target);
            if ((lastPosition - camera->position).lengthSq() > EPS || lastZoom != camera->zoom) {
                lastPosition.copy(camera->position);
                lastZoom = camera->zoom;
            }
        }
    }

    void sizeChanged(Vector4& screen)
    {
        this->screen.copy(screen);
        //PerspectiveCamera* perspectiveCamera = dynamic_cast<PerspectiveCamera*>(camera);        
        //if (perspectiveCamera!=nullptr) {            
        //    perspectiveCamera->aspect = screen.z/screen.w;            
        //} 
        camera->updateProjectionMatrix();
    }

    void reset() 
    {
        state = STATE::NONE;
        keyState = STATE::NONE;

        target.copy(target0);
        camera->position.copy(position0);
        camera->up.copy(up0);
        camera->zoom = zoom0;

        camera->updateProjectionMatrix();

        eye.subVectors(camera->position, target);

        camera->lookAt(target);

        //_this.dispatchEvent(changeEvent);

        lastPosition.copy(camera->position);
        lastZoom = camera->zoom;
    }

    void keyDown(Key key) 
    {
        if (scope->enabled == false) return;

        //window.removeEventListener('keydown', keydown);

        if (keyState != STATE::NONE) {

            return;

        } else if (key == Key::A && !scope->noRotate) {

            keyState = STATE::ROTATE;

        } else if (key == Key::S && !scope->noZoom) {

            keyState = STATE::ZOOM;

        } else if (key == Key::D && !scope->noPan) {

            keyState = STATE::PAN;
        }
    }

    void keyUp()
    {
        if (scope->enabled == false) return;

        keyState = STATE::NONE;
    }


    void mouseDown(unsigned button, float x, float y)
    {
        if (!scope->enabled) return;


        if (state == STATE::NONE) {
            switch (button) {
                case 0://MouseButtons.Left
                    state = STATE::ROTATE;
                    break;
                case 2://MouseButtons.Middle:
                    state = STATE::ZOOM;
                    break;
                case 1://MouseButtons.Right:
                    state = STATE::PAN;
                    break;
                default:
                    state = STATE::NONE;
            }
        }

        if (state == STATE::ROTATE && !scope->noRotate) {

            rotateStart = getMouseProjectionOnBall(x, y);
            rotateEnd = rotateStart;

        } else if (state == STATE::ZOOM && !scope->noZoom) {

            zoomStart = getMouseOnScreen(x, y);
            zoomEnd = zoomStart;


        } else if (state == STATE::PAN && !scope->noPan) {

            panStart = getMouseOnScreen(x, y);
            panEnd = panStart;
        }
    }

    void mouseMove(float x, float y)
    {
        if (!scope->enabled) return;

        if (state == STATE::ROTATE && !scope->noRotate) {

            rotateEnd = getMouseProjectionOnBall(x, y);

        } else if (state == STATE::ZOOM && !scope->noZoom) {

            zoomEnd = getMouseOnScreen(x, y);

        } else if (state == STATE::PAN && !scope->noPan) {

            panEnd = getMouseOnScreen(x, y);
        }
    }

    void mouseUp()
    {
        if (!scope->enabled) return;
        state = STATE::NONE;
    }

    void mouseWheel(float delta)
    {
        if (!scope->enabled) return;
        if (scope->noZoom) return;
        float _delta = delta;

        zoomStart.y += _delta * 0.01f;
    }

    void touchstart() {}

    void touchmove() {}

    void touchend() {}

    void onMouseMove(const Vector2& pos) override {
        mouseMove(pos.x, pos.y);
    }
    void onMouseDown(int button, const Vector2& pos) override {
        mouseDown(button, pos.x, pos.y);
    }
    void onMouseUp(int button, const Vector2& pos) override {
        mouseUp();
    }
    void onMouseWheel(const Vector2& delta) override {
        mouseWheel(delta.y);
    }
    void onKeyRepeat(KeyEvent evt) override {
        keyDown(evt.key);
    }
    void onKeyReleased(KeyEvent evt) override {
        keyUp();
    }
    void setState(int mouseState) {
        this->state = (STATE) mouseState;
    }
    ~Impl() override {
        deactivate();
    }
};

TrackballControls::TrackballControls(Camera& camera, Vector4& screen, PeripheralsEventSource& canvas) 
    : pimpl_(std::make_unique<Impl>(*this, camera, screen, canvas)) {}

void TrackballControls::initTrackballControls(Camera& camera, Vector4& screen) {
    pimpl_->initTrackballControls(camera, screen);
}

void TrackballControls::update() {
    pimpl_->update();
}

void TrackballControls::sizeChanged(Vector4& screen) {
    pimpl_->sizeChanged(screen);
}

TrackballControls::~TrackballControls() = default;
