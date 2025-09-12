#pragma once

#include "threepp/lines/LineSegmentsGeometry.hpp"
#include "threepp/core/BufferAttribute.hpp"
#include "threepp/core/BufferGeometry.hpp"
#include "threepp/objects/Line.hpp"
#include <memory>
#include <string>
#include <vector>

namespace threepp {

    class LineGeometry: public LineSegmentsGeometry {
    public:
        std::string type;

        LineGeometry();

        LineGeometry& setPositions(const std::vector<float>& array);
        LineGeometry& setColors(const std::vector<float>& array);
        LineGeometry& fromLine(const std::shared_ptr<Line>& line);

        static std::shared_ptr<LineGeometry> create() {
            return std::make_shared<LineGeometry>();
        }
    };

}// namespace threepp