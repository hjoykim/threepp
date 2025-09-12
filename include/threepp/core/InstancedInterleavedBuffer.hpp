#pragma once

#include "threepp/core/InterleavedBuffer.hpp"
#include <vector>
#include <memory>
namespace threepp {

    class InstancedInterleavedBuffer: public InterleavedBuffer {
    public:
        int meshPerAttribute{1};

        InstancedInterleavedBuffer() = default;

        InstancedInterleavedBuffer(const std::vector<float>& array, int stride, int meshPerAttribute = 1)
            : InterleavedBuffer(array, stride), meshPerAttribute(meshPerAttribute) {
            this->updateRange = UpdateRange{0, -1};
        }
           
        static std::shared_ptr<InstancedInterleavedBuffer> create(const std::vector<float>& array, int stride, int meshPerAttribute = 1) {
            return std::make_shared<InstancedInterleavedBuffer>(array, stride, meshPerAttribute);
        }
    };

}// namespace threepp