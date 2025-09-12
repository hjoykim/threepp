#include "threepp/core/InstancedBufferGeometry.hpp"

namespace threepp {

    InstancedBufferGeometry::InstancedBufferGeometry()
        : BufferGeometry() {}

    InstancedBufferGeometry::InstancedBufferGeometry(const InstancedBufferGeometry& source)
        : BufferGeometry(),
          groups(source.groups),
          maxInstanceCount(source.maxInstanceCount),
          instanceCount(source.instanceCount) {}

    std::unique_ptr<InstancedBufferGeometry> InstancedBufferGeometry::clone() const {
        return std::make_unique<InstancedBufferGeometry>(*this);
    }

    InstancedBufferGeometry& InstancedBufferGeometry::copy(const InstancedBufferGeometry& source) {
        BufferGeometry::copy(source);
        groups = source.groups;
        maxInstanceCount = source.maxInstanceCount;
        instanceCount = source.instanceCount;
        return *this;
    }

    void InstancedBufferGeometry::addGroup(int start, int count, int instances) {
        groups.emplace_back(start, count, instances);
    }

}// namespace threepp