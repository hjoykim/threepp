#pragma once

#include "threepp/math/Box3.hpp"
#include "threepp/core/BufferGeometry.hpp"
#include "threepp/core/InstancedInterleavedBuffer.hpp"
#include "threepp/core/InterleavedBufferAttribute.hpp"
#include "threepp/core/Raycaster.hpp"
#include "threepp/math/Line3.hpp"
#include "threepp/materials/Material.hpp"
#include "threepp/math/Matrix4.hpp"
#include "threepp/objects/Mesh.hpp"
#include "threepp/math/Sphere.hpp"
#include "threepp/math/Vector3.hpp"
#include "threepp/math/Vector4.hpp"
#include <memory>
#include <vector>

namespace threepp {

    class LineSegments2: public Mesh {
    private:
        Vector3 _start;
        Vector3 _end;
        Vector4 _start4;
        Vector4 _end4;
        Vector4 _ssOrigin;
        Vector3 _ssOrigin3;
        Matrix4 _mvMatrix;
        Line3 _line;
        Vector3 _closestPoint;
        Box3 _box;
        Sphere _sphere;
        Vector4 _clipToWorldVector;

    public:
        LineSegments2(std::shared_ptr<BufferGeometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr);

        LineSegments2& computeLineDistances();

        void raycast(Raycaster& raycaster, std::vector<Intersection>& intersects);

        static std::shared_ptr<LineSegments2> create(std::shared_ptr<BufferGeometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr) {
            return std::make_shared<LineSegments2>(geometry, material);
        }
    };

}// namespace threepp