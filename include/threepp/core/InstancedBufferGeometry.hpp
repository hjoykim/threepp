#pragma once

#include "threepp/core/BufferGeometry.hpp"
#include <limits>
#include <optional>
#include <vector>

namespace threepp {

    struct InstancedGroups {
        int start;
        int count;
        int instances;

        InstancedGroups(int start, int count, int instances)
            : start(start), count(count), instances(instances) {}
    };

    class InstancedBufferGeometry: public BufferGeometry {
    public:
        std::vector<InstancedGroups> groups;
        std::optional<int> maxInstanceCount = std::nullopt;
        int instanceCount{std::numeric_limits<int>::max()};

        InstancedBufferGeometry();

        InstancedBufferGeometry(const InstancedBufferGeometry& source);

        std::unique_ptr<InstancedBufferGeometry> clone() const;

        InstancedBufferGeometry& copy(const InstancedBufferGeometry& source);

        void addGroup(int start, int count, int instances);

        static std::shared_ptr<InstancedBufferGeometry> create() {
            return std::make_shared<InstancedBufferGeometry>();
        }
    };

}// namespace threepp