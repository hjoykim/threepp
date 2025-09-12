#pragma once

#include "threepp/core/BufferAttribute.hpp"
#include <memory>

namespace threepp {

    class InstancedBufferAttribute: public FloatBufferAttribute {
    public:
        int meshPerAttribute{1};               

        InstancedBufferAttribute() = default;

        InstancedBufferAttribute(const std::vector<float>& array, int itemSize, bool normalized = false, int meshPerAttribute = 1)
            : FloatBufferAttribute(array, itemSize, normalized), meshPerAttribute(meshPerAttribute) {}

        InstancedBufferAttribute(std::initializer_list<float>&& array, int itemSize, bool normalized = false, int meshPerAttribute = 1)
            : FloatBufferAttribute(array, itemSize, normalized), meshPerAttribute(meshPerAttribute) {}
             
        std::unique_ptr<InstancedBufferAttribute> clone() const {
            auto clone = std::make_unique<InstancedBufferAttribute>(*this);
            return clone;
        }

        InstancedBufferAttribute& copy(const InstancedBufferAttribute& source) 
        {
            FloatBufferAttribute::copy(source);
            this->meshPerAttribute = source.meshPerAttribute;
            return *this;
        }

        static std::shared_ptr<InstancedBufferAttribute> create() {
            return std::make_shared<InstancedBufferAttribute>();
        }

        static std::shared_ptr<InstancedBufferAttribute> create(const std::vector<float>& array, int itemSize, bool normalized = false, int meshPerAttribute = 1) {
            return std::make_shared<InstancedBufferAttribute>(array, itemSize, normalized, meshPerAttribute);
        }

        template<std::ranges::range Range>
        static std::shared_ptr<InstancedBufferAttribute> create(const Range& range, int itemSize, bool normalized = false, int meshPerAttribute = 1) {
            return std::make_shared<InstancedBufferAttribute>(std::vector<float>(std::ranges::begin(range), std::ranges::end(range)), itemSize, normalized, meshPerAttribute);
        }

        static std::shared_ptr<InstancedBufferAttribute> create(std::initializer_list<float>&& array, int itemSize, bool normalized = false, int meshPerAttribute = 1) {
            return std::make_shared<InstancedBufferAttribute>(std::vector<float>{array.begin(), array.end()}, itemSize, normalized, meshPerAttribute);
        }
        ~InstancedBufferAttribute() override = default;

    };
}// namespace threepp