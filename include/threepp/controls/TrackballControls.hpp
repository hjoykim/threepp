#ifndef THREEPP_TRACKBALLCONTROLS_HPP
#define THREEPP_TRACKBALLCONTROLS_HPP
#include "threepp/math/MathUtils.hpp"
#include <limits>
#include <memory>

namespace threepp {
    class PeripheralsEventSource;
    class Camera;
    struct WindowSize;
    class Vector4;
    class TrackballControls {
    
    public:

        bool enabled = true;

        float rotateSpeed = 1.0f;
        float zoomSpeed = 1.2f;
        float panSpeed = 0.3f;

        bool noRotate = false;
        bool noZoom = false;
        bool noPan = false;
        bool noRoll = false;

        bool staticMoving = false;
        float dynamicDampingFactor = 0.2f;

        float minDistance = 0.0f;
        float maxDistance = std::numeric_limits<float>::infinity();

        TrackballControls(Camera& camera, Vector4& screen, PeripheralsEventSource& canvas);

        void initTrackballControls(Camera& camera, Vector4& screen); 

        void update();

        void sizeChanged(Vector4& screen);

        ~TrackballControls();

    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl_;
        
    };

}// namespace threepp
#endif//THREEPP_TRACKBALLCONTROLS_HPP