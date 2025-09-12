#pragma once

#include "threepp/math/Color.hpp"
#include "threepp/materials/ShaderMaterial.hpp"
#include "threepp/math/Vector2.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace threepp {

    class LineMaterial: public ShaderMaterial {
    public:

        [[nodiscard]] std::string type() const override;
        LineMaterial();
        static std::shared_ptr<LineMaterial> create();

        // Uniforms
        Color color() const;
        void setColor(const Color& value);

        float lineWidth() const;
        void setLineWidth(float value);

        bool dashed() const;
        void setDashed(bool value);

        float dashScale() const;
        void setDashScale(float value);

        float dashSize() const;
        void setDashSize(float value);

        float dashOffset() const;
        void setDashOffset(float value);

        float gapSize() const;
        void setGapSize(float value);

        float opacity() const;
        void setOpacity(float value);

        Vector2 resolution() const;
        void setResolution(const Vector2& value);

        bool alphaToCoverage() const;
        void setAlphaToCoverage(bool value);

        // Shader code
        static const std::string vertexShader_;
        static const std::string fragmentShader_;
    };

}// namespace threepp