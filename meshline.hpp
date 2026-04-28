#pragma once

#include "threepp/core/BufferGeometry.hpp"
#include "threepp/core/BufferAttribute.hpp"
#include "threepp/math/Matrix4.hpp"
#include "threepp/math/Vector3.hpp"
#include "threepp/core/Raycaster.hpp"
#include <functional>
#include <memory>
#include <vector>

namespace threepp {

    class MeshLine: public BufferGeometry {
    public:
        MeshLine();

        static std::shared_ptr<MeshLine> create();

        // set a geometry (BufferGeometry) or raw points
        void setGeometry(const std::shared_ptr<BufferGeometry>& g, std::function<float(float)> widthCb = {});
        void setPoints(const std::vector<float>& points, std::function<float(float)> widthCb = {});
        void setPoints(const std::vector<Vector3>& points, std::function<float(float)> widthCb = {});

        // advance by one new point (fast rolling update)
        void advance(const Vector3& position);

        // build attributes from internal arrays
        void process();

        // raycast helper that mirrors original MeshLineRaycast behavior
        void raycast(const Raycaster& raycaster, std::vector<Intersection>& intersects) const;

        // allow setting a matrixWorld (JS MeshLine kept a matrixWorld for raycast)
        void setMatrixWorld(const Matrix4& matrixWorld);

    private:
        // CPU-side arrays used to build BufferAttributes
        std::vector<float> positions_; // duplicated per vertex: two vertices per point -> 6 floats per point
        std::vector<float> previous_;
        std::vector<float> next_;
        std::vector<float> side_;
        std::vector<float> width_;
        std::vector<int> indices_array_;
        std::vector<float> uvs_;
        std::vector<float> counters_;

        std::shared_ptr<BufferGeometry> _geom_; // if user passed a geometry
        std::function<float(float)> widthCallback_;

        Matrix4 matrixWorld_; // used by raycast
    };

}// namespace threepp