#include "threepp/lines/MeshLineMaterial.hpp"
#include "threepp/renderers/shaders/ShaderChunk.hpp"
#include "threepp/renderers/shaders/UniformsLib.hpp"

using namespace threepp;

const std::string MeshLineMaterial::vertexShader_ = R"(
attribute vec3 previous;
attribute vec3 next;
attribute float side;
attribute float width;
attribute float counters;
uniform vec2 resolution;
uniform float lineWidth;
uniform float sizeAttenuation;
uniform vec3 color;
uniform float opacity;
varying vec2 vUV;
varying vec4 vColor;
varying float vCounters;
vec2 fix(vec4 i, float aspect) {
    vec2 res = i.xy / i.w;
    res.x *= aspect;
    vCounters = counters;
    return res;
}
void main() {
    float aspect = resolution.x / resolution.y;
    vColor = vec4(color, opacity);
    vUV = uv;
    mat4 m = projectionMatrix * modelViewMatrix;
    vec4 finalPosition = m * vec4(position, 1.0);
    vec4 prevPos = m * vec4(previous, 1.0);
    vec4 nextPos = m * vec4(next, 1.0);
    vec2 currentP = fix(finalPosition, aspect);
    vec2 prevP = fix(prevPos, aspect);
    vec2 nextP = fix(nextPos, aspect);
    float w = lineWidth * width;
    vec2 dir;
    if(nextP == currentP) dir = normalize(currentP - prevP);
    else if(prevP == currentP) dir = normalize(nextP - currentP);
    else {
        vec2 dir1 = normalize(currentP - prevP);
        vec2 dir2 = normalize(nextP - currentP);
        dir = normalize(dir1 + dir2);
    }
    vec4 normal = vec4(-dir.y, dir.x, 0., 1.);
    normal.xy *= .5 * w;
    normal *= projectionMatrix;
    if(sizeAttenuation == 0.) {
        normal.xy *= finalPosition.w;
        normal.xy /= (vec4(resolution, 0., 1.) * projectionMatrix).xy;
    }
    finalPosition.xy += normal.xy * side;
    gl_Position = finalPosition;
}
)";

const std::string MeshLineMaterial::fragmentShader_ = R"(
uniform sampler2D map;
uniform float useMap;
uniform float useAlphaMap;
uniform float useDash;
uniform float dashArray;
uniform float dashOffset;
uniform float dashRatio;
uniform float visibility;
uniform float alphaTest;
uniform vec2 repeat;
varying vec2 vUV;
varying vec4 vColor;
varying float vCounters;
void main() {
    vec4 c = vColor;
    gl_FragColor = c;
    gl_FragColor.a *= step(vCounters, visibility);
    if (gl_FragColor.a < alphaTest) discard;
}
)";

MeshLineMaterial::MeshLineMaterial()
    : ShaderMaterial() {

    // create uniforms similar to original implementation
    UniformMap u = UniformsLib::instance().fog; // copy fog uniforms as base
    u["lineWidth"] = Uniform(1.f);
    u["map"] = Uniform(nullptr);
    u["useMap"] = Uniform(0.f);
    u["alphaMap"] = Uniform(nullptr);
    u["useAlphaMap"] = Uniform(0.f);
    u["color"] = Uniform(Color(0xffffff));
    u["opacity"] = Uniform(1.f);
    u["resolution"] = Uniform(Vector2(1.f, 1.f));
    u["sizeAttenuation"] = Uniform(1.f);
    u["dashArray"] = Uniform(0.f);
    u["dashOffset"] = Uniform(0.f);
    u["dashRatio"] = Uniform(0.5f);
    u["useDash"] = Uniform(0.f);
    u["visibility"] = Uniform(1.f);
    u["alphaTest"] = Uniform(0.f);
    u["repeat"] = Uniform(Vector2(1.f, 1.f));

    this->uniforms = std::move(u);

    this->vertexShader = MeshLineMaterial::vertexShader_;
    this->fragmentShader = MeshLineMaterial::fragmentShader_;
}

std::shared_ptr<MeshLineMaterial> MeshLineMaterial::create() {
    return std::make_shared<MeshLineMaterial>();
}

float MeshLineMaterial::lineWidth() const {
    return uniforms.at("lineWidth").value<float>();
}
void MeshLineMaterial::setLineWidth(float value) {
    uniforms.at("lineWidth").setValue(value);
}

Color MeshLineMaterial::color() const {
    return uniforms.at("color").value<Color>();
}
void MeshLineMaterial::setColor(const Color& value) {
    uniforms.at("color").value<Color>().copy(value);
}

float MeshLineMaterial::opacity() const {
    return uniforms.at("opacity").value<float>();
}
void MeshLineMaterial::setOpacity(float value) {
    uniforms.at("opacity").setValue(value);
}

Vector2 MeshLineMaterial::resolution() const {
    return uniforms.at("resolution").value<Vector2>();
}
void MeshLineMaterial::setResolution(const Vector2& value) {
    uniforms.at("resolution").value<Vector2>().copy(value);
}

float MeshLineMaterial::sizeAttenuation() const {
    return uniforms.at("sizeAttenuation").value<float>();
}
void MeshLineMaterial::setSizeAttenuation(float v) {
    uniforms.at("sizeAttenuation").setValue(v);
}

void MeshLineMaterial::setDashArray(float v) {
    uniforms.at("dashArray").setValue(v);
    uniforms.at("useDash").setValue(v != 0.f ? 1.f : 0.f);
}
float MeshLineMaterial::dashArray() const {
    return uniforms.at("dashArray").value<float>();
}
void MeshLineMaterial::setUseDash(bool v) {
    uniforms.at("useDash").setValue(v ? 1.f : 0.f);
}
bool MeshLineMaterial::useDash() const {
    return uniforms.at("useDash").value<float>() != 0.f;
}