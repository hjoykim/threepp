#include "threepp/lines/MeshLine.hpp"
#include "threepp/core/BufferAttribute.hpp"
#include "threepp/math/Sphere.hpp"
#include "threepp/math/Matrix4.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace threepp;

MeshLine::MeshLine()
    : BufferGeometry(),
      matrixWorld_(Matrix4()) {}

std::shared_ptr<MeshLine> MeshLine::create() {
    return std::make_shared<MeshLine>();
}

void MeshLine::setMatrixWorld(const Matrix4& matrixWorld) {
    this->matrixWorld_ = matrixWorld;
}

void MeshLine::setGeometry(const std::shared_ptr<BufferGeometry>& g, std::function<float(float)> widthCb) {
    _geom_ = g;
    if (!g) return;
    // expects position attribute present
    if (auto posAttr = g->getAttribute<float>("position")) {
        setPoints(posAttr->array(), widthCb);
    }
}

void MeshLine::setPoints(const std::vector<float>& points, std::function<float(float)> widthCb) {
    widthCallback_ = widthCb;
    positions_.clear();
    counters_.clear();

    // points is flat float array x,y,z,...
    const auto len = points.size();
    if (len % 3 != 0) {
        std::cerr << "MeshLine::setPoints: points length must be multiple of 3\n";
        return;
    }
    const unsigned int n = static_cast<unsigned int>(len / 3);
    for (unsigned int j = 0; j < n; ++j) {
        const float x = points[3 * j + 0];
        const float y = points[3 * j + 1];
        const float z = points[3 * j + 2];

        // duplicate positions per point (two vertices per point)
        positions_.push_back(x); positions_.push_back(y); positions_.push_back(z);
        positions_.push_back(x); positions_.push_back(y); positions_.push_back(z);

        const float c = static_cast<float>(3 * j) / static_cast<float>(len);
        counters_.push_back(c);
        counters_.push_back(c);
    }
    process();
}

void MeshLine::setPoints(const std::vector<Vector3>& points, std::function<float(float)> widthCb) {
    widthCallback_ = widthCb;
    positions_.clear();
    counters_.clear();

    const unsigned int n = static_cast<unsigned int>(points.size());
    for (unsigned int j = 0; j < n; ++j) {
        const auto& p = points[j];
        positions_.push_back(p.x); positions_.push_back(p.y); positions_.push_back(p.z);
        positions_.push_back(p.x); positions_.push_back(p.y); positions_.push_back(p.z);

        const float c = static_cast<float>(j) / static_cast<float>(n);
        counters_.push_back(c);
        counters_.push_back(c);
    }
    process();
}

void MeshLine::process() {
    const int l = static_cast<int>(positions_.size() / 6); // points count

    previous_.clear();
    next_.clear();
    side_.clear();
    width_.clear();
    indices_array_.clear();
    uvs_.clear();

    std::vector<float> pts; // helper to copy V3
    auto copyV3 = [&](int idx)->std::array<float,3> {
        int base = idx * 6;
        return { positions_[base], positions_[base + 1], positions_[base + 2] };
    };
    auto compareV3 = [&](int a, int b)->bool {
        auto aa = a * 6;
        auto ab = b * 6;
        return positions_[aa] == positions_[ab] && positions_[aa + 1] == positions_[ab + 1] && positions_[aa + 2] == positions_[ab + 2];
    };

    // initial previous
    std::array<float,3> v;
    if (compareV3(0, l - 1)) {
        v = copyV3(l - 2);
    } else {
        v = copyV3(0);
    }
    previous_.push_back(v[0]); previous_.push_back(v[1]); previous_.push_back(v[2]);
    previous_.push_back(v[0]); previous_.push_back(v[1]); previous_.push_back(v[2]);

    for (int j = 0; j < l; ++j) {
        // sides
        side_.push_back(1.f);
        side_.push_back(-1.f);

        // widths
        float w = 1.f;
        if (widthCallback_) w = widthCallback_(static_cast<float>(j) / std::max(1, l - 1));
        width_.push_back(w); width_.push_back(w);

        // uvs
        const float u = static_cast<float>(j) / std::max(1, l - 1);
        uvs_.push_back(u); uvs_.push_back(0.f);
        uvs_.push_back(u); uvs_.push_back(1.f);

        if (j < l - 1) {
            v = copyV3(j);
            previous_.push_back(v[0]); previous_.push_back(v[1]); previous_.push_back(v[2]);
            previous_.push_back(v[0]); previous_.push_back(v[1]); previous_.push_back(v[2]);

            int n = j * 2;
            indices_array_.push_back(n); indices_array_.push_back(n + 1); indices_array_.push_back(n + 2);
            indices_array_.push_back(n + 2); indices_array_.push_back(n + 1); indices_array_.push_back(n + 3);
        }
        if (j > 0) {
            v = copyV3(j);
            next_.push_back(v[0]); next_.push_back(v[1]); next_.push_back(v[2]);
            next_.push_back(v[0]); next_.push_back(v[1]); next_.push_back(v[2]);
        }
    }

    // last next
    if (compareV3(l - 1, 0)) {
        v = copyV3(1);
    } else {
        v = copyV3(l - 1);
    }
    next_.push_back(v[0]); next_.push_back(v[1]); next_.push_back(v[2]);
    next_.push_back(v[0]); next_.push_back(v[1]); next_.push_back(v[2]);

    // create or update attributes
    this->setAttribute("position", FloatBufferAttribute::create(positions_, 3));
    this->setAttribute("previous", FloatBufferAttribute::create(previous_, 3));
    this->setAttribute("next", FloatBufferAttribute::create(next_, 3));
    this->setAttribute("side", FloatBufferAttribute::create(side_, 1));
    this->setAttribute("width", FloatBufferAttribute::create(width_, 1));
    this->setAttribute("uv", FloatBufferAttribute::create(uvs_, 2));
    this->setAttribute("counters", FloatBufferAttribute::create(counters_, 1));

    // indices
    this->setIndex(indices_array_);

    this->computeBoundingSphere();
    this->computeBoundingBox();
}

void MeshLine::advance(const Vector3& position) {
    auto posAttr = this->getAttribute<float>("position");
    auto prevAttr = this->getAttribute<float>("previous");
    auto nextAttr = this->getAttribute<float>("next");

    if (!posAttr || !prevAttr || !nextAttr) return;

    auto& positions = posAttr->array();
    auto& previous = prevAttr->array();
    auto& next = nextAttr->array();

    const int l = static_cast<int>(positions.size());

    // PREVIOUS <- positions[0..]
    std::copy(positions.begin(), positions.end(), previous.begin());

    // POSITIONS shift left by 6
    if (l > 6) {
        std::move(positions.begin() + 6, positions.end(), positions.begin());
    }

    // set last two verts to new position
    positions[l - 6] = position.x;
    positions[l - 5] = position.y;
    positions[l - 4] = position.z;
    positions[l - 3] = position.x;
    positions[l - 2] = position.y;
    positions[l - 1] = position.z;

    // NEXT
    if (l > 6) {
        std::copy(positions.begin() + 6, positions.end(), next.begin());
    }

    next[l - 6] = position.x;
    next[l - 5] = position.y;
    next[l - 4] = position.z;
    next[l - 3] = position.x;
    next[l - 2] = position.y;
    next[l - 1] = position.z;

    posAttr->needsUpdate();
    prevAttr->needsUpdate();
    nextAttr->needsUpdate();
}

void MeshLine::raycast(const Raycaster& raycaster, std::vector<Intersection>& intersects) const {
    // This mirrors the JS MeshLineRaycast logic.
    auto geometry = std::dynamic_pointer_cast<BufferGeometry>(const_cast<MeshLine*>(this)->shared_from_this());
    if (!geometry) return;

    if (!geometry->boundingSphere) {
        const_cast<MeshLine*>(this)->computeBoundingSphere();
    }

    Sphere sphere = *geometry->boundingSphere;
    sphere.applyMatrix4(matrixWorld_);

    if (!raycaster.ray.intersectsSphere(sphere)) return;

    Matrix4 inverseMatrix = matrixWorld_;
    inverseMatrix.invert();

    Ray ray = raycaster.ray;
    ray.applyMatrix4(inverseMatrix);

    Vector3 vStart, vEnd, interSegment, interRay;

    auto index = geometry->getIndex();
    auto positions = geometry->getAttribute<float>("position");
    auto widths = geometry->getAttribute<float>("width");

    if (index && positions) {
        const auto& indices = index->array();
        const auto& posArr = positions->array();
        const auto& widthArr = widths ? widths->array() : std::vector<float>{};

        for (unsigned i = 0, l = static_cast<unsigned>(indices.size()) - 1; i < l; i += 1) {
            auto a = indices[i];
            auto b = indices[i + 1];

            vStart.set(posArr[a * 3 + 0], posArr[a * 3 + 1], posArr[a * 3 + 2]);
            vEnd.set(posArr[b * 3 + 0], posArr[b * 3 + 1], posArr[b * 3 + 2]);

            float width = 1.0f;
            if (!widthArr.empty()) {
                int wi = static_cast<int>(std::floor(i / 3.0f));
                if (wi >= 0 && wi < static_cast<int>(widthArr.size())) width = widthArr[wi];
            }
            float precision = raycaster.params.lineThreshold + width / 2.0f;
            float precisionSq = precision * precision;

            float distSq = ray.distanceSqToSegment(vStart, vEnd, &interRay, &interSegment);

            if (distSq > precisionSq) continue;

            interRay.applyMatrix4(matrixWorld_);

            float distance = raycaster.ray.origin.distanceTo(interRay);

            if (distance < raycaster.nearPlane || distance > raycaster.farPlane) continue;

            Intersection inter;
            inter.distance = distance;
            inter.point = interSegment.clone().applyMatrix4(matrixWorld_);
            inter.index = i;
            inter.face = std::nullopt;
            inter.faceIndex = std::nullopt;
            inter.object = nullptr; // geometry only; user can map to object
            intersects.push_back(inter);

            // only first hit
            break;
        }
    }
}