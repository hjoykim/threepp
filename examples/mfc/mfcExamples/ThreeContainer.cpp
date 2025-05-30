#include "ThreeContainer.h"

WindowSize CThreeContainer::size() const
{
	return WindowSize{width, height};
}

CThreeContainer::CThreeContainer(int x, int y, void* hwnd) : width(x), height(y), hwnd(hwnd)
{
	camera = &pcamera;
	renderer = std::make_shared<GLRenderer>(WindowSize{width,height});
}

void CThreeContainer::setHwnd(int x, int y, void* hwnd) 
{
	camera = &pcamera;
	width = x;
	height = y;
	this->hwnd = hwnd;
	if (renderer) renderer.reset();
	renderer = std::make_shared<GLRenderer>(WindowSize{ width,height });

	initRenderer();

	if (uiPtr && Initialized) {
		uiPtr.reset();
		setUIControls();
	}
}

void CThreeContainer::initCamera()
{
	pcamera.position.set(-30, 40, 30);
	pcamera.lookAt(Vector3(0, 0, 0));
	pcamera.updateProjectionMatrix();
}

void CThreeContainer::initLighting()
{
}

void CThreeContainer::initControls()
{
	controls = std::make_shared<OrbitControls>(pcamera, *this);
}

void CThreeContainer::initRenderer()
{
	renderer->setClearColor(Color(0xEEEEEE));
	renderer->shadowMap().enabled = true;
	renderer->shadowMap().type = ShadowMap::PFCSoft;
}

void CThreeContainer::init()
{
	if (!Initialized) {
		initRenderer();
		initCamera();
		initControls();
		initLighting();
		Initialized = true;
		sizeChanged(width, height);
	}
}

void CThreeContainer::render()
{
	renderer->render(scene, *camera);
	if (uiPtr) {
		uiPtr->render();
	}
}

void CThreeContainer::sizeChanged(int w, int h)
{
	width = w;
	height = h;
	float aspect = (float)width / (float)height;
	renderer->setSize({ width, height });
	pcamera.aspect = aspect;
	pcamera.updateProjectionMatrix();
	renderer->setViewport(0, 0, width, height);
}

void CThreeContainer::setUIControls()
{
}








