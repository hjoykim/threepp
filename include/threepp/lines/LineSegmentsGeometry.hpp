#pragma once

#include "threepp/core/InstancedBufferGeometry.hpp"
#include "threepp/math/Box3.hpp"
#include "threepp/core/InstancedInterleavedBuffer.hpp"
#include "threepp/core/InterleavedBufferAttribute.hpp"
#include "threepp/math/Sphere.hpp"
#include "threepp/math/Vector3.hpp"
#include "threepp/objects/Mesh.hpp"
#include "threepp/objects/LineSegments.hpp"
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace threepp {

    class LineSegmentsGeometry: public InstancedBufferGeometry {
    private:
        Box3 _box;
        Vector3 _vector;

    public:
        LineSegmentsGeometry();

        LineSegmentsGeometry& applyMatrix(const Matrix4& matrix);
        LineSegmentsGeometry& applyMatrix4(const Matrix4& matrix);

        LineSegmentsGeometry& setPositions(const std::vector<float>& array);
        LineSegmentsGeometry& setColors(const std::vector<float>& array);

        LineSegmentsGeometry& fromWireframeGeometry(const std::shared_ptr<BufferGeometry>& geometry);
        LineSegmentsGeometry& fromEdgesGeometry(const std::shared_ptr<BufferGeometry>& geometry);
        LineSegmentsGeometry& fromMesh(const std::shared_ptr<Mesh>& mesh);
        LineSegmentsGeometry& fromLineSegments(const std::shared_ptr<LineSegments>& lineSegments);

        void computeBoundingBox();
        void computeBoundingSphere();
    };

}// namespace threepp