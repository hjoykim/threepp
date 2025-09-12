#include "threepp/lines/LineGeometry.hpp"
#include "threepp/core/BufferAttribute.hpp"
#include "threepp/objects/Line.hpp"

namespace threepp {

    LineGeometry::LineGeometry()
        : LineSegmentsGeometry(), type("LineGeometry") {}

    LineGeometry& LineGeometry::setPositions(const std::vector<float>& array) {
        size_t length = array.size() - 3;
        std::vector<float> points(2 * length);

        for (size_t i = 0; i < length; i += 3) {
            points[2 * i] = array[i];
            points[2 * i + 1] = array[i + 1];
            points[2 * i + 2] = array[i + 2];

            points[2 * i + 3] = array[i + 3];
            points[2 * i + 4] = array[i + 4];
            points[2 * i + 5] = array[i + 5];
        }

        LineSegmentsGeometry::setPositions(points);
        return *this;
    }

    LineGeometry& LineGeometry::setColors(const std::vector<float>& array) {
        size_t length = array.size() - 3;
        std::vector<float> colors(2 * length);

        for (size_t i = 0; i < length; i += 3) {
            colors[2 * i] = array[i];
            colors[2 * i + 1] = array[i + 1];
            colors[2 * i + 2] = array[i + 2];

            colors[2 * i + 3] = array[i + 3];
            colors[2 * i + 4] = array[i + 4];
            colors[2 * i + 5] = array[i + 5];
        }

        LineSegmentsGeometry::setColors(colors);
        return *this;
    }

    LineGeometry& LineGeometry::fromLine(const std::shared_ptr<Line>& line) {
        auto geometry = std::dynamic_pointer_cast<BufferGeometry>(line->geometry());
        if (geometry) {
            auto positionAttr = geometry->getAttribute("position");
            if (!positionAttr) return *this;
            auto attr = positionAttr->typed<float>();
            if (attr) {
                setPositions(attr->array());
            }
        }
        // set colors, maybe
        return *this;
    }

}// namespace threepp