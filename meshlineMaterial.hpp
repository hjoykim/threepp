#pragma once

#include "threepp/materials/ShaderMaterial.hpp"
#include "threepp/math/Vector2.hpp"
#include "threepp/math/Color.hpp"

namespace threepp {

    class MeshLineMaterial: public ShaderMaterial {
    public:
        MeshLineMaterial();

        static std::shared_ptr<MeshLineMaterial> create();

        // Uniform accessors (subset from original JS)
        float lineWidth() const;
        void setLineWidth(float value);

        Color color() const;
        void setColor(const Color& value);

        float opacity() const;
        void setOpacity(float value);

        Vector2 resolution() const;
        void setResolution(const Vector2& value);

        float sizeAttenuation() const;
        void setSizeAttenuation(float v);

        // dash / visibility etc.
        void setDashArray(float v);
        float dashArray() const;
        void setUseDash(bool v);
        bool useDash() const;

        // simplified shaders are embedded here (can be replaced/refined)
        static const std::string vertexShader_;
        static const std::string fragmentShader_;
    };

}// namespace threepp