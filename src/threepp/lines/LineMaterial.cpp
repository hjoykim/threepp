#include "threepp/lines/LineMaterial.hpp"
#include "threepp/core/Shader.hpp"
#include "threepp/core/Uniform.hpp"
#include "threepp/renderers/gl/GLUniforms.hpp"
#include "threepp/renderers/shaders/UniformsLib.hpp"
namespace threepp {

    const std::string LineMaterial::vertexShader_ = R"(
        #include <common>
		#include <color_pars_vertex>
		#include <fog_pars_vertex>
		#include <logdepthbuf_pars_vertex>
		#include <clipping_planes_pars_vertex>

		uniform float linewidth;
		uniform vec2 resolution;

		attribute vec3 instanceStart;
		attribute vec3 instanceEnd;

		attribute vec3 instanceColorStart;
		attribute vec3 instanceColorEnd;

		varying vec2 vUv;

		#ifdef USE_DASH

			uniform float dashScale;
			attribute float instanceDistanceStart;
			attribute float instanceDistanceEnd;
			varying float vLineDistance;

		#endif

		void trimSegment( const in vec4 start, inout vec4 end ) {

			// trim end segment so it terminates between the camera plane and the near plane

			// conservative estimate of the near plane
			float a = projectionMatrix[ 2 ][ 2 ]; // 3nd entry in 3th column
			float b = projectionMatrix[ 3 ][ 2 ]; // 3nd entry in 4th column
			float nearEstimate = - 0.5 * b / a;

			float alpha = ( nearEstimate - start.z ) / ( end.z - start.z );

			end.xyz = mix( start.xyz, end.xyz, alpha );

		}

		void main() {

			#ifdef USE_COLOR

				vColor.xyz = ( position.y < 0.5 ) ? instanceColorStart : instanceColorEnd;

			#endif

			#ifdef USE_DASH

				vLineDistance = ( position.y < 0.5 ) ? dashScale * instanceDistanceStart : dashScale * instanceDistanceEnd;

			#endif

			float aspect = resolution.x / resolution.y;

			vUv = uv;

			// camera space
			vec4 start = modelViewMatrix * vec4( instanceStart, 1.0 );
			vec4 end = modelViewMatrix * vec4( instanceEnd, 1.0 );

			// special case for perspective projection, and segments that terminate either in, or behind, the camera plane
			// clearly the gpu firmware has a way of addressing this issue when projecting into ndc space
			// but we need to perform ndc-space calculations in the shader, so we must address this issue directly
			// perhaps there is a more elegant solution -- WestLangley

			bool perspective = ( projectionMatrix[ 2 ][ 3 ] == - 1.0 ); // 4th entry in the 3rd column

			if ( perspective ) {

				if ( start.z < 0.0 && end.z >= 0.0 ) {

					trimSegment( start, end );

				} else if ( end.z < 0.0 && start.z >= 0.0 ) {

					trimSegment( end, start );

				}

			}

			// clip space
			vec4 clipStart = projectionMatrix * start;
			vec4 clipEnd = projectionMatrix * end;

			// ndc space
			vec2 ndcStart = clipStart.xy / clipStart.w;
			vec2 ndcEnd = clipEnd.xy / clipEnd.w;

			// direction
			vec2 dir = ndcEnd - ndcStart;

			// account for clip-space aspect ratio
			dir.x *= aspect;
			dir = normalize( dir );

			// perpendicular to dir
			vec2 offset = vec2( dir.y, - dir.x );

			// undo aspect ratio adjustment
			dir.x /= aspect;
			offset.x /= aspect;

			// sign flip
			if ( position.x < 0.0 ) offset *= - 1.0;

			// endcaps
			if ( position.y < 0.0 ) {

				offset += - dir;

			} else if ( position.y > 1.0 ) {

				offset += dir;

			}

			// adjust for linewidth
			offset *= linewidth;

			// adjust for clip-space to screen-space conversion // maybe resolution should be based on viewport ...
			offset /= resolution.y;

			// select end
			vec4 clip = ( position.y < 0.5 ) ? clipStart : clipEnd;

			// back to clip space
			offset *= clip.w;

			clip.xy += offset;

			gl_Position = clip;

			vec4 mvPosition = ( position.y < 0.5 ) ? start : end; // this is an approximation

			#include <logdepthbuf_vertex>
			#include <clipping_planes_vertex>
			#include <fog_vertex>

		}
    )";

    const std::string LineMaterial::fragmentShader_ = R"(
        uniform vec3 diffuse;
		uniform float opacity;

		#ifdef USE_DASH

			uniform float dashSize;
			uniform float dashOffset;
			uniform float gapSize;

		#endif

		varying float vLineDistance;

		#include <common>
		#include <color_pars_fragment>
		#include <fog_pars_fragment>
		#include <logdepthbuf_pars_fragment>
		#include <clipping_planes_pars_fragment>

		varying vec2 vUv;

		void main() {

			#include <clipping_planes_fragment>

			#ifdef USE_DASH

				if ( vUv.y < - 1.0 || vUv.y > 1.0 ) discard; // discard endcaps

				if ( mod( vLineDistance + dashOffset, dashSize + gapSize ) > dashSize ) discard; // todo - FIX

			#endif

			float alpha = opacity;

			#ifdef ALPHA_TO_COVERAGE

			// artifacts appear on some hardware if a derivative is taken within a conditional
			float a = vUv.x;
			float b = ( vUv.y > 0.0 ) ? vUv.y - 1.0 : vUv.y + 1.0;
			float len2 = a * a + b * b;
			float dlen = fwidth( len2 );

			if ( abs( vUv.y ) > 1.0 ) {

				alpha = 1.0 - smoothstep( 1.0 - dlen, 1.0 + dlen, len2 );

			}

			#else

			if ( abs( vUv.y ) > 1.0 ) {

				float a = vUv.x;
				float b = ( vUv.y > 0.0 ) ? vUv.y - 1.0 : vUv.y + 1.0;
				float len2 = a * a + b * b;

				if ( len2 > 1.0 ) discard;

			}

			#endif

			vec4 diffuseColor = vec4( diffuse, alpha );

			#include <logdepthbuf_fragment>
			#include <color_fragment>

			gl_FragColor = vec4( diffuseColor.rgb, alpha );

			#include <tonemapping_fragment>
			#include <encodings_fragment>
			#include <fog_fragment>
			#include <premultiplied_alpha_fragment>

		}
    )";

    LineMaterial::LineMaterial()
        : ShaderMaterial() {



        // Uniforms 초기화
        uniforms["linewidth"] = Uniform(1.0f);
        uniforms["resolution"] = Uniform(Vector2(1, 1));
        uniforms["dashed"] = Uniform(false);
        uniforms["dashScale"] = Uniform(1.0f);
        uniforms["dashSize"] = Uniform(1.0f);
        uniforms["dashOffset"] = Uniform(0.0f);
        uniforms["gapSize"] = Uniform(1.0f);
        uniforms["opacity"] = Uniform(1.0f);
        uniforms.insert(shaders::UniformsLib::instance().common.begin(),
                         shaders::UniformsLib::instance().common.end());;
        uniforms.insert(shaders::UniformsLib::instance().fog.begin(),
                        shaders::UniformsLib::instance().fog.end());
       
        this->vertexShader = vertexShader_;
        this->fragmentShader = fragmentShader_;
        this->clipping = true;
    }
    std::string LineMaterial::type() const {

        return "LineMaterial";
    }
    std::shared_ptr<LineMaterial> LineMaterial::create() {
        return std::make_shared<LineMaterial>();
    }
    // Getter/Setter 구현 예시
    Color LineMaterial::color() const {
        const auto& u = uniforms.at("diffuse");
        return const_cast<Uniform&>(u).value<Color>();
    }
    void LineMaterial::setColor(const Color& value) {
        uniforms.at("diffuse").setValue(value);
    }

    float LineMaterial::lineWidth() const {
        const auto& u = uniforms.at("linewidth");
        return const_cast<Uniform&>(u).value<float>();
    }
    void LineMaterial::setLineWidth(float value) {
        uniforms.at("linewidth").setValue(value);
    }

    bool LineMaterial::dashed() const {
        return defines.count("USE_DASH") > 0;
    }
    void LineMaterial::setDashed(bool value) {
        if ((defines.count("USE_DASH") > 0) != value) {
            needsUpdate();
        }
        if (value) {
            defines["USE_DASH"] = "";
        } else {
            defines.erase("USE_DASH");
        }
    }

    float LineMaterial::dashScale() const {
        const auto& u = uniforms.at("dashScale");
        return const_cast<Uniform&>(u).value<float>();
    }
    void LineMaterial::setDashScale(float value) {
        uniforms.at("dashScale").setValue(value);
    }

    float LineMaterial::dashSize() const {
        const auto& u = uniforms.at("dashSize");	
		return const_cast<Uniform&>(u).value<float>();
    }
    void LineMaterial::setDashSize(float value) {
        uniforms.at("dashSize").setValue(value);
    }

    float LineMaterial::dashOffset() const {
        const auto& u = uniforms.at("dashOffset");
        return const_cast<Uniform&>(u).value<float>();
    }
    void LineMaterial::setDashOffset(float value) {
        uniforms.at("dashOffset").setValue(value);
    }

    float LineMaterial::gapSize() const {
        const auto& u = uniforms.at("gapSize");
        return const_cast<Uniform&>(u).value<float>();
    }
    void LineMaterial::setGapSize(float value) {
        uniforms.at("gapSize").setValue(value);
    }

    float LineMaterial::opacity() const {
        const auto& u = uniforms.at("opacity");
        return const_cast<Uniform&>(u).value<float>();
    }
    void LineMaterial::setOpacity(float value) {
        uniforms.at("opacity").setValue(value);
    }

    Vector2 LineMaterial::resolution() const {
        const auto& u = uniforms.at("resolution");
        return const_cast<Uniform&>(u).value<Vector2>();
    }
    void LineMaterial::setResolution(const Vector2& value) {
        uniforms.at("resolution").setValue(value);
    }

    bool LineMaterial::alphaToCoverage() const {

        return defines.count("ALPHA_TO_COVERAGE") > 0;
    }
    void LineMaterial::setAlphaToCoverage(bool value) {
        if ((defines.count("ALPHA_TO_COVERAGE") > 0) != value) {
            needsUpdate();
        }
        if (value) {
            defines["ALPHA_TO_COVERAGE"] = "";
        } else {
            defines.erase("ALPHA_TO_COVERAGE");
        }
    }

}// namespace threepp