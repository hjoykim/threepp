#pragma once
#include <string>
#include <memory>
#include <vector>
#include "ThreeContainer.h"
#include "FirstSceneExample.h"
#include "MaterialLightAnimation.h"
class CLoadExamples {
public:
	struct Example {
		std::string exampleName;
		std::shared_ptr<CThreeContainer> example;
	};
	std::vector<Example> examples;
	CLoadExamples(int width, int height, void* hwnd=nullptr) {
		examples.push_back(Example{ "First-Scene", std::dynamic_pointer_cast<CThreeContainer>(std::make_shared<CFirstSceneExample>(width,height)) });
		examples.push_back(Example{ "Material-Light-Animation", std::dynamic_pointer_cast<CThreeContainer>(std::make_shared<CMaterialLightAnimation>(width,height,hwnd)) });
	}

	~CLoadExamples() = default;

};
