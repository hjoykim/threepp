#pragma once
#include <threepp/threepp.hpp>
#include <threepp/input/PeripheralsEventSource.hpp>
#include <threepp/extras/imgui/ImguiContext.hpp>
using namespace threepp;
class CThreeContainer : public PeripheralsEventSource{

public:
	void* hwnd; // 
	bool Initialized = false;
	IOCapture capture;
	std::shared_ptr<ImguiFunctionalContext> uiPtr;
	
	[[nodiscard]] WindowSize size() const override;
	Scene scene;
	Camera* camera;
	PerspectiveCamera pcamera;
	OrthographicCamera ocamera;
	std::shared_ptr<OrbitControls> controls;
	std::shared_ptr<GLRenderer> renderer = nullptr;
	int width;
	int height;
	bool isPerspective;
	CThreeContainer(){};
	CThreeContainer(int x,int y, void* hwnd=nullptr);
	~CThreeContainer() = default;	
	void setHwnd(int x, int y, void* hwnd);
	virtual void initCamera();
	virtual void initLighting();
	virtual void initControls();
	virtual void initRenderer();
	virtual void init();
	virtual void render();
	virtual void sizeChanged(int w, int h);
	virtual void setUIControls();
};

