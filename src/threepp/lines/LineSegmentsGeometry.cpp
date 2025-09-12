#include "threepp/lines/LineSegmentsGeometry.hpp"
#include "threepp/core/BufferAttribute.hpp"
#include "threepp/core/InstancedInterleavedBuffer.hpp"
#include "threepp/core/InterleavedBufferAttribute.hpp"
#include "threepp/objects/LineSegments.hpp"
#include "threepp/math/Matrix4.hpp"
#include "threepp/objects/Mesh.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace threepp {

    LineSegmentsGeometry::LineSegmentsGeometry()
        : InstancedBufferGeometry() {

        std::vector<float> positions = {-1, 2, 0, 1, 2, 0, -1, 1, 0, 1, 1, 0, -1, 0, 0, 1, 0, 0, -1, -1, 0, 1, -1, 0};
        std::vector<float> uvs = {-1, 2, 1, 2, -1, 1, 1, 1, -1, -1, 1, -1, -1, -2, 1, -2};
        std::vector<int> index = {0, 2, 1, 2, 3, 1, 2, 4, 3, 4, 5, 3, 4, 6, 5, 6, 7, 5};

        this->setIndex(index);
        this->setAttribute("position", FloatBufferAttribute::create(positions, 3));
        this->setAttribute("uv", FloatBufferAttribute::create(uvs, 2));
    }

    LineSegmentsGeometry& LineSegmentsGeometry::applyMatrix(const Matrix4& matrix) {
        return applyMatrix4(matrix);
    }

    LineSegmentsGeometry& LineSegmentsGeometry::applyMatrix4(const Matrix4& matrix) {
        auto start = dynamic_cast<InterleavedBufferAttribute*>(this->getAttribute("instanceStart"));
        auto end = dynamic_cast<InterleavedBufferAttribute*>(this->getAttribute("instanceEnd"));
        if (start && end) {
            start->applyMatrix4(matrix);
            end->applyMatrix4(matrix);
            start->needsUpdate();
        }
        if (this->boundingBox) {
            this->computeBoundingBox();
        }
        if (this->boundingSphere) {
            this->computeBoundingSphere();
        }
        return *this;
    }

    LineSegmentsGeometry& LineSegmentsGeometry::setPositions(const std::vector<float>& array) {
        auto instanceBuffer = InstancedInterleavedBuffer::create(array, 6, 1);
        this->setAttribute("instanceStart", InterleavedBufferAttribute::create(instanceBuffer, 3, 0));
        this->setAttribute("instanceEnd", InterleavedBufferAttribute::create(instanceBuffer, 3, 3));
        this->computeBoundingBox();
        this->computeBoundingSphere();
        return *this;
    }

    LineSegmentsGeometry& LineSegmentsGeometry::setColors(const std::vector<float>& array) {
        auto instanceColorBuffer = InstancedInterleavedBuffer::create(array, 6, 1);
        this->setAttribute("instanceColorStart", InterleavedBufferAttribute::create(instanceColorBuffer, 3, 0));
        this->setAttribute("instanceColorEnd", InterleavedBufferAttribute::create(instanceColorBuffer, 3, 3));
        return *this;
    }

    LineSegmentsGeometry& LineSegmentsGeometry::fromWireframeGeometry(const std::shared_ptr<BufferGeometry>& geometry) {
        if (!geometry) return *this;
        auto attr = geometry->getAttribute("position")->typed<float>();
        
        if (attr) {
            this->setPositions(attr->array());
        }
        return *this;
    }

    LineSegmentsGeometry& LineSegmentsGeometry::fromEdgesGeometry(const std::shared_ptr<BufferGeometry>& geometry) {
        if (!geometry) return *this;
        auto attr = geometry->getAttribute("position")->typed<float>();
        if (attr) {
            this->setPositions(attr->array());
        }
        return *this;
    }

    LineSegmentsGeometry& LineSegmentsGeometry::fromMesh(const std::shared_ptr<Mesh>& mesh) {
        if (!mesh) return *this;
        auto geometry = std::dynamic_pointer_cast<BufferGeometry>(mesh->geometry());
        if (geometry) {
            this->fromWireframeGeometry(geometry);
        }
        return *this;
    }

    LineSegmentsGeometry& LineSegmentsGeometry::fromLineSegments(const std::shared_ptr<LineSegments>& lineSegments) {
        if (!lineSegments) return *this;
        auto geometry = std::dynamic_pointer_cast<BufferGeometry>(lineSegments->geometry());
        if (geometry) {
            auto attr = geometry->getAttribute("position")->typed<float>();
            if (attr) {
                this->setPositions(attr->array());
            }
        }
        return *this;
    }

    void LineSegmentsGeometry::computeBoundingBox() {
        if (!this->boundingBox) {
            this->boundingBox = Box3();
        }
        auto start = dynamic_cast<InterleavedBufferAttribute*>(this->getAttribute("instanceStart"));
        auto end = dynamic_cast<InterleavedBufferAttribute*>(this->getAttribute("instanceEnd"));
        if (start && end) {
            this->boundingBox->setFromBufferAttribute(*start);
            _box.setFromBufferAttribute(*end);
            this->boundingBox->union_(_box);
        }
    }

    void LineSegmentsGeometry::computeBoundingSphere() {
        if (!this->boundingSphere) {
            this->boundingSphere = Sphere();
        }
        if (!this->boundingBox) {
            this->computeBoundingBox();
        }
        auto start = dynamic_cast<InterleavedBufferAttribute*>(this->getAttribute("instanceStart"));
        auto end = dynamic_cast<InterleavedBufferAttribute*>(this->getAttribute("instanceEnd"));
        if (start && end) {
            auto& center = this->boundingSphere->center;
            this->boundingBox->getCenter(center);
            float maxRadiusSq = 0;
            int count = start->count();
            for (int i = 0; i < count; ++i) {
                _vector.fromBufferAttribute(*start, i);
                maxRadiusSq = std::max(maxRadiusSq, center.distanceToSquared(_vector));
                _vector.fromBufferAttribute(*end, i);
                maxRadiusSq = std::max(maxRadiusSq, center.distanceToSquared(_vector));
            }
            this->boundingSphere->radius = std::sqrt(maxRadiusSq);
            if (std::isnan(this->boundingSphere->radius)) {
                std::cerr << "threepp::LineSegmentsGeometry::computeBoundingSphere(): Computed radius is NaN. The instanced position data is likely to have NaN values." << std::endl;
                std::exit(-1);
            }
        }
    }

}// namespace threepp