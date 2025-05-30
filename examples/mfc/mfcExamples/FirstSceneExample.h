#pragma once
#include "ThreeContainer.h"
class CFirstSceneExample : public CThreeContainer {
public:
	CFirstSceneExample() {}
	CFirstSceneExample(int width,int height, void* hwnd) : CThreeContainer(width, height,hwnd)
	{
		scene.background = Background(Color(0xEEEEEE));       
	}
	~CFirstSceneExample() = default;
	virtual void initRenderer() override;
	virtual void init() override;
};
