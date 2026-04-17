#pragma once
#include <optional>
#include "threepp/constants.hpp"
#include "threepp/renderers/gl/GLState.hpp"
#include "threepp/renderers/gl/GLShadowMap.hpp"
#include "threepp/canvas/WindowSize.hpp"
namespace threepp {

    class GLRenderTarget;
    class Object3D;
    class Camera;
    class Scene;
    class BufferGeometry;
    class Material;    
    class IGLRenderer {        
    public:
        bool autoClear = true;
        bool autoClearColor = true;
        bool autoClearDepth = true;
        bool autoClearStencil = true;

        bool sortObjects = true;

        std::vector<Plane> clippingPlanes;
        bool localClippingEnabled = false;
        
        float gammaFactor{2.0f};        
        Encoding outputEncoding{Encoding::Linear};

        bool physicallyCorrectLights = false;

        ToneMapping toneMapping{ToneMapping::None};
        float toneMappingExposure = 1.0f;

        bool checkShaderErrors = false;

        virtual gl::GLState& state() = 0;
        virtual gl::GLShadowMap& shadowMap() = 0;
        [[nodiscard]] virtual  const gl::GLShadowMap& shadowMap() const = 0;

        [[nodiscard]] virtual int getTargetPixelRatio() const = 0;
        virtual void setPixelRatio(int value) = 0;

        [[nodiscard]] virtual WindowSize size() const = 0;

        virtual void setSize(const std::pair<int, int>& size) = 0;

        virtual void getViewport(Vector4& target) const = 0;

        virtual void setViewport(const Vector4& v) = 0;
        virtual void setViewport(int x, int y, int width, int height) = 0;
        virtual void clear(bool color = true, bool depth = true, bool stencil = true) = 0;
        virtual void dispose()=0;

        virtual void render(Object3D& scene, Camera& camera) = 0;
        virtual void renderBufferDirect(Camera* camera, Scene* scene, BufferGeometry* geometry, Material* material, Object3D* object, std::optional<GeometryGroup> group)=0;
        
        [[nodiscard]] virtual int getActiveCubeFace() const = 0;
        [[nodiscard]] virtual int getActiveMipmapLevel() const = 0;

        virtual GLRenderTarget* getRenderTarget() = 0;
        virtual void setRenderTarget(GLRenderTarget* renderTarget, int activeCubeFace = 0, int activeMipmapLevel = 0) = 0;        
        
        virtual void resetState() = 0;
        //virtual [[nodiscard]] const gl::GLInfo& info() const = 0;

        virtual ~IGLRenderer() = default;   
    };
}