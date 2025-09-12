// https://github.com/mrdoob/three.js/blob/r129/src/core/InterleavedBufferAttribute.js

#ifndef THREEPP_INTERLEAVEDBUFFERATTRIBUTE_HPP
#define THREEPP_INTERLEAVEDBUFFERATTRIBUTE_HPP

#include <memory>
#include <vector>

#include "threepp/core/BufferAttribute.hpp"
#include "threepp/core/InterleavedBuffer.hpp"

namespace threepp {

    class InterleavedBufferAttribute: public FloatBufferAttribute {

    public:
        unsigned int offset;
        std::shared_ptr<InterleavedBuffer> data;

        InterleavedBufferAttribute(std::shared_ptr<InterleavedBuffer> data, int itemSize, unsigned int offset, bool normalized)
            : data(std::move(data)), offset(offset), TypedBufferAttribute({}, itemSize, normalized) {}

        static std::shared_ptr<InterleavedBufferAttribute> create(std::shared_ptr<InterleavedBuffer> data, int itemSize, unsigned int offset, bool normalized = false) {
            return std::make_shared<InterleavedBufferAttribute>(data, itemSize, offset, normalized);
        }

        [[nodiscard]] std::vector<float>& array() override {

            return data->array();
        }

        [[nodiscard]] const std::vector<float>& array() const override {

            return data->array();
        }

        [[nodiscard]] int count() const override {

            return data->count();
        }

        InterleavedBufferAttribute& setX(size_t index, float x) override {

            this->data->array()[index * this->data->stride() + this->offset] = x;

            return *this;
        }

        InterleavedBufferAttribute& setY(size_t index, float y) override {

            this->data->array()[index * this->data->stride() + this->offset + 1] = y;

            return *this;
        }

        InterleavedBufferAttribute& setZ(size_t index, float z) override {

            this->data->array()[index * this->data->stride() + this->offset + 2] = z;

            return *this;
        }

        InterleavedBufferAttribute& setW(size_t index, float w) override {

            this->data->array()[index * this->data->stride() + this->offset + 3] = w;

            return *this;
        }

        [[nodiscard]] float getX(size_t index) const override {

            return this->data->array()[index * this->data->stride() + this->offset];
        }

        [[nodiscard]] float getY(size_t index) const override {

            return this->data->array()[index * this->data->stride() + this->offset + 1];
        }

        [[nodiscard]] float getZ(size_t index) const override {

            return this->data->array()[index * this->data->stride() + this->offset + 2];
        }

        [[nodiscard]] float getW(size_t index) const override {

            return this->data->array()[index * this->data->stride() + this->offset + 3];
        }

        InterleavedBufferAttribute& setXY(size_t index, float x, float y) override {

            index = index * this->data->stride() + this->offset;

            this->data->array()[index + 0] = x;
            this->data->array()[index + 1] = y;

            return *this;
        }

        InterleavedBufferAttribute& setXYZ(size_t index, float x, float y, float z) override {

            index = index * this->data->stride() + this->offset;

            this->data->array()[index + 0] = x;
            this->data->array()[index + 1] = y;
            this->data->array()[index + 2] = z;

            return *this;
        }

        InterleavedBufferAttribute& setXYZW(size_t index, float x, float y, float z, float w) override {

            index = index * this->data->stride() + this->offset;

            this->data->array()[index + 0] = x;
            this->data->array()[index + 1] = y;
            this->data->array()[index + 2] = z;
            this->data->array()[index + 3] = w;

            return *this;
        }
    };

}// namespace threepp

#endif//THREEPP_INTERLEAVEDBUFFERATTRIBUTE_HPP
