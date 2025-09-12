#include "threepp/lines/Line2.hpp"

namespace threepp {
    Line2::Line2(const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material) : LineSegments2(geometry, material) {}
    std::shared_ptr<Line2> Line2::create(const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material) {
        return std::make_shared<Line2>(geometry, material);
    }
}// namespace threepp