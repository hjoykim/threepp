#pragma once
#include "ThreeContainer.h"
#include "threepp/materials/materials.hpp"
#include "threepp/lines/Line2.hpp"
#include "threepp/lines/LineGeometry.hpp"
class FatLineExample : public CThreeContainer {
public:

	FatLineExample(int width, int height, void* hwnd);
 
	~FatLineExample() = default;
	virtual void initRenderer() override;
    virtual void initCamera() override;
	virtual void init() override;
    virtual void sizeChanged(int w, int h) override;
    virtual void setUIControls() override;
    virtual void render() override;
    PerspectiveCamera camera2;
    std::shared_ptr<LineMaterial> matLine;

private:
    int insetWidth, insetHeight;
   
    std::shared_ptr<LineBasicMaterial> matLineBasic;
    std::shared_ptr<LineDashedMaterial> matLineDashed;
    std::shared_ptr<Line2> line;
    std::shared_ptr<Line> line1;
    int lineType = 0;
    float lineWidth;
    bool alphaToCoverage = true;
    bool dashed = false;
    float dashScale = 1;
    int dashGap = 1;
    void createObjects();
};
