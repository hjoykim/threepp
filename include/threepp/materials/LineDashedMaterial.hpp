#pragma once

#include "threepp/materials/LineBasicMaterial.hpp"

namespace threepp {

    class LineDashedMaterial: public LineBasicMaterial {

    public:
        float scale{1.f};
        float dashSize{3.f};
        float gapSize{1.f};

        [[nodiscard]] std::string type() const override;

        static std::shared_ptr<LineDashedMaterial> create(const std::unordered_map<std::string, MaterialValue>& values = {});

    protected:
        LineDashedMaterial();
        std::shared_ptr<Material> createDefault() const override;
        void copyInto(Material& material) const override;
        bool setValue(const std::string& key, const MaterialValue& value) override;
    };

}// namespace threepp