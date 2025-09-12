#pragma once

#include "threepp/math/Vector3.hpp"
#include <vector>

namespace threepp {

    class GeometryUtils {
    public:
        // center 기본값은 Vector3() -> (0,0,0)
        static std::vector<Vector3> Hilbert3D(const Vector3& center = Vector3(),
                                              float size = 10.0f,
                                              int iterations = 1,
                                              int v0 = 0, int v1 = 1, int v2 = 2, int v3 = 3,
                                              int v4 = 4, int v5 = 5, int v6 = 6, int v7 = 7);
    };

}// namespace threepp