#include "threepp/threepp.hpp"
#include "threepp/lines/LineSegments2.hpp"
#include "threepp/math/MathUtils.hpp"
#include "threepp/core/InterleavedBufferAttribute.hpp"
#include "threepp/core/InstancedInterleavedBuffer.hpp"
#include "threepp/lines/LineMaterial.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace threepp {

    LineSegments2::LineSegments2(std::shared_ptr<BufferGeometry> geometry, std::shared_ptr<Material> material)
        : Mesh(std::move(geometry), std::move(material)) {}

    LineSegments2& LineSegments2::computeLineDistances() {
        auto geometry = std::dynamic_pointer_cast<BufferGeometry>(this->geometry_);
        auto instanceStart = dynamic_cast<InterleavedBufferAttribute*>(geometry->getAttribute("instanceStart"));
        auto instanceEnd = dynamic_cast<InterleavedBufferAttribute*>(geometry->getAttribute("instanceEnd"));
        std::vector<float> lineDistances(2 * instanceStart->count());

        for (int i = 0, j = 0, l = instanceStart->count(); i < l; i++, j += 2) {
            _start.fromBufferAttribute(*instanceStart, i);
            _end.fromBufferAttribute(*instanceEnd, i);

            lineDistances[j] = (j == 0) ? 0 : lineDistances[j - 1];
            lineDistances[j + 1] = lineDistances[j] + _start.distanceTo(_end);
        }

        auto instanceDistanceBuffer = InstancedInterleavedBuffer::create(lineDistances, 2, 1);
        auto distStartAttr = InterleavedBufferAttribute::create(instanceDistanceBuffer, 1, 0);
        auto distEndAttr = InterleavedBufferAttribute::create(instanceDistanceBuffer, 1, 1);

        geometry->setAttribute("instanceDistanceStart", distStartAttr);
        geometry->setAttribute("instanceDistanceEnd", distEndAttr);

        return *this;
    }

    void LineSegments2::raycast(Raycaster& raycaster, std::vector<Intersection>& intersects) {
        if (!raycaster.camera) {
            std::cerr << "LineSegments2: 'Raycaster.camera' needs to be set in order to raycast against LineSegments2." << std::endl;
            std::exit(-1);
        }
        auto geometry = std::dynamic_pointer_cast<BufferGeometry>(this->geometry_);
        if (!geometry) return;

        auto lineMat = material().get();
        if (!lineMat) return;

        auto instanceStart = dynamic_cast<InterleavedBufferAttribute*>(geometry->getAttribute("instanceStart"));
        auto instanceEnd = dynamic_cast<InterleavedBufferAttribute*>(geometry->getAttribute("instanceEnd"));
        if (!instanceEnd || !instanceStart) return;

        float threshold = 0.0f;
        if (raycaster.params.lineThreshold) threshold = raycaster.params.lineThreshold;

        auto& ray = raycaster.ray;
        auto camera = raycaster.camera;
        auto& projectionMatrix = camera->projectionMatrix;
        auto& matrixWorld = *this->matrixWorld;
        
        auto resolution = lineMat->as<LineMaterial>()->resolution();
        float lineWidth = lineMat->as<LineMaterial>()->lineWidth() + threshold;

       

        float near = -camera->nearPlane;
        float ssMaxWidth = 2.0f * std::max(lineWidth / resolution.x, lineWidth / resolution.y);

        if (!geometry->boundingSphere) {
            geometry->computeBoundingSphere();
        }

        _sphere.copy(*geometry->boundingSphere).applyMatrix4(matrixWorld);
        float distanceToSphere = std::max(camera->nearPlane, _sphere.distanceToPoint(ray.origin));

        _clipToWorldVector.set(0, 0, -distanceToSphere, 1.0f).applyMatrix4(camera->projectionMatrix);
        _clipToWorldVector.multiplyScalar(1.0f / _clipToWorldVector.w);
        _clipToWorldVector.applyMatrix4(camera->projectionMatrixInverse);

        float sphereMargin = std::abs(ssMaxWidth / _clipToWorldVector.w) * 0.5f;
        _sphere.radius += sphereMargin;

        if (!ray.intersectsSphere(_sphere)) {
            return;
        }

        if (!geometry->boundingBox) {
            geometry->computeBoundingBox();
        }

        _box.copy(*geometry->boundingBox).applyMatrix4(matrixWorld);
        float distanceToBox = std::max(camera->nearPlane, _box.distanceToPoint(ray.origin));

        _clipToWorldVector.set(0, 0, -distanceToBox, 1.0f).applyMatrix4(camera->projectionMatrix);
        _clipToWorldVector.multiplyScalar(1.0f / _clipToWorldVector.w);
        _clipToWorldVector.applyMatrix4(camera->projectionMatrixInverse);

        float boxMargin = std::abs(ssMaxWidth / _clipToWorldVector.w) * 0.5f;
        _box.setMax(Vector3(_box.max().x + boxMargin, _box.max().y + boxMargin, _box.max().z + boxMargin));
        _box.setMin(Vector3(_box.min().x - boxMargin,_box.min().y - boxMargin, _box.min().z - boxMargin));

        if (!ray.intersectsBox(_box)) {
            return;
        }

        ray.at(1, _ssOrigin);
        _ssOrigin.w = 1.0f;
        _ssOrigin.applyMatrix4(camera->matrixWorldInverse);
        _ssOrigin.applyMatrix4(projectionMatrix);
        _ssOrigin.multiplyScalar(1.0f / _ssOrigin.w);

        _ssOrigin.x *= resolution.x / 2.0f;
        _ssOrigin.y *= resolution.y / 2.0f;
        _ssOrigin.z = 0.0f;

        _ssOrigin3.set(_ssOrigin.x, _ssOrigin.y, _ssOrigin.z);

        _mvMatrix.multiplyMatrices(camera->matrixWorldInverse, matrixWorld);

        const size_t count = instanceStart->count();
        for (int i = 0; i < count; i++) {
            _start4.fromBufferAttribute(*instanceStart, i);
            _end4.fromBufferAttribute(*instanceEnd, i);

            _start4.w = 1;
            _end4.w = 1;

            _start4.applyMatrix4(_mvMatrix);
            _end4.applyMatrix4(_mvMatrix);

            bool isBehindCameraNear = _start4.z > near && _end4.z > near;
            if (isBehindCameraNear) continue;

            if (_start4.z > near) {
                float deltaDist = _start4.z - _end4.z;
                float t = (_start4.z - near) / deltaDist;
                _start4.lerp(_end4, t);
            } else if (_end4.z > near) {
                float deltaDist = _end4.z - _start4.z;
                float t = (_end4.z - near) / deltaDist;
                _end4.lerp(_start4, t);
            }

            _start4.applyMatrix4(projectionMatrix);
            _end4.applyMatrix4(projectionMatrix);

            _start4.multiplyScalar(1.0f / _start4.w);
            _end4.multiplyScalar(1.0f / _end4.w);

            _start4.x *= resolution.x / 2;
            _start4.y *= resolution.y / 2;
            _end4.x *= resolution.x / 2;
            _end4.y *= resolution.y / 2;

            _line.set(Vector3(_start4.x, _start.y, 0.0f), Vector3(_end4.x, _end4.y, 0.0f));
            

            float param = _line.closestPointToPointParameter(_ssOrigin3, true);
            _line.at(param, _closestPoint);

            float zPos = threepp::math::lerp(_start4.z, _end4.z, param);
            bool isInClipSpace = zPos >= -1 && zPos <= 1;
            bool isInside = _ssOrigin3.distanceTo(_closestPoint) < lineWidth * 0.5f;

            if (isInClipSpace && isInside) {
                Vector3 start;
                start.fromBufferAttribute(*instanceStart, static_cast<int>(i));

                Vector3 end;
                end.fromBufferAttribute(*instanceEnd, static_cast<int>(i));
                start.applyMatrix4(matrixWorld);
                end.applyMatrix4(matrixWorld);
                _line.set(start, end);

                Vector3 pointOnLine, point;
                ray.distanceSqToSegment(_line.start(), _line.end(), &point, &pointOnLine);

                Intersection intersection;
                intersection.point = point;
                intersection.pointOnLine = pointOnLine;
                intersection.distance = ray.origin.distanceTo(point);
                intersection.object = this;
                intersection.face = std::nullopt;
                intersection.faceIndex = i;
                intersection.uv = std::nullopt;
                intersection.uv2 = std::nullopt;

                intersects.push_back(intersection);
            }
        }
    }

}// namespace threepp