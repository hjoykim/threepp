#include "threepp/utils/GeometryUtils.hpp"
#include <vector>

namespace threepp {

    std::vector<Vector3> GeometryUtils::Hilbert3D(const Vector3& center,
                                                  float size,
                                                  int iterations,
                                                  int v0, int v1, int v2, int v3,
                                                  int v4, int v5, int v6, int v7) {

        float half = size / 2.0f;

        std::vector<Vector3> vec_s{
                Vector3(center.x - half, center.y + half, center.z - half),
                Vector3(center.x - half, center.y + half, center.z + half),
                Vector3(center.x - half, center.y - half, center.z + half),
                Vector3(center.x - half, center.y - half, center.z - half),
                Vector3(center.x + half, center.y - half, center.z - half),
                Vector3(center.x + half, center.y - half, center.z + half),
                Vector3(center.x + half, center.y + half, center.z + half),
                Vector3(center.x + half, center.y + half, center.z - half)};

        std::vector<Vector3> vec{
                vec_s[v0],
                vec_s[v1],
                vec_s[v2],
                vec_s[v3],
                vec_s[v4],
                vec_s[v5],
                vec_s[v6],
                vec_s[v7]};

        // Recurse iterations
        if (--iterations >= 0) {
            std::vector<Vector3> tmp;

            auto append = [&](std::vector<Vector3>& dst, std::vector<Vector3> src) {
                dst.insert(dst.end(), std::make_move_iterator(src.begin()), std::make_move_iterator(src.end()));
            };

            append(tmp, Hilbert3D(vec[0], half, iterations, v0, v3, v4, v7, v6, v5, v2, v1));
            append(tmp, Hilbert3D(vec[1], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3));
            append(tmp, Hilbert3D(vec[2], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3));
            append(tmp, Hilbert3D(vec[3], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5));
            append(tmp, Hilbert3D(vec[4], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5));
            append(tmp, Hilbert3D(vec[5], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7));
            append(tmp, Hilbert3D(vec[6], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7));
            append(tmp, Hilbert3D(vec[7], half, iterations, v6, v5, v2, v1, v0, v3, v4, v7));

            return tmp;
        }

        // Return complete Hilbert Curve.
        return vec;
    }

}// namespace threepp