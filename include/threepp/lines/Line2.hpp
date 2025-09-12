#pragma once

#include "threepp/lines/LineSegments2.hpp"
#include <memory>

namespace threepp {

    class Line2: public LineSegments2 {
    public:
        Line2(const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material);
        static std::shared_ptr<Line2> create(const std::shared_ptr<BufferGeometry>& geometry = nullptr, const std::shared_ptr<Material>& material = nullptr);
    };

}// namespace threepp