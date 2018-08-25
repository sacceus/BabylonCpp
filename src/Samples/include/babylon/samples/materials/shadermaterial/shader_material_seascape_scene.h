#ifndef BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_SEASCAPE_SCENE_H
#define BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_SEASCAPE_SCENE_H

#include <babylon/interfaces/irenderable_scene.h>

namespace BABYLON {
namespace Samples {

class ShaderMaterialSeascapeScene : public IRenderableScene {

public:
  /** Vertex Shader **/
  static constexpr const char* customVertexShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "// Attributes\n"
      "attribute vec3 position;\n"
      "attribute vec2 uv;\n"
      "\n"
      "// Uniforms\n"
      "uniform mat4 worldViewProjection;\n"
      "\n"
      "// Varying\n"
      "varying vec2 vUV;\n"
      "\n"
      "void main(void) {\n"
      "    gl_Position = worldViewProjection * vec4(position, 1.0);\n"
      "    vUV = uv;\n"
      "}\n";

  /** Pixel (Fragment) Shader **/
  // Seascape ( https://www.shadertoy.com/view/Ms2SD1 )
  static constexpr const char* customFragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "// Varying\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "// Uniforms\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float iTime;\n"
      "uniform float iAspectRatio;\n"
      "uniform vec2 iResolution;\n"
      "\n"
      "/*\n"
      " * \"Seascape\" by Alexander Alekseev aka TDM - 2014\n"
      " * License Creative Commons Attribution-NonCommercial-ShareAlike 3.0\n"
      " * Unported\n"
      " * License.\n"
      " * Contact: tdmaav@gmail.com\n"
      " */\n"
      "\n"
      "const int NUM_STEPS = 8;\n"
      "const float PI      = 3.141592;\n"
      "const float EPSILON = 1e-3;\n"
      "#define EPSILON_NRM (0.1 / iResolution.x)\n"
      "\n"
      "// sea\n"
      "const int ITER_GEOMETRY    = 3;\n"
      "const int ITER_FRAGMENT    = 5;\n"
      "const float SEA_HEIGHT     = 0.6;\n"
      "const float SEA_CHOPPY     = 4.0;\n"
      "const float SEA_SPEED      = 0.8;\n"
      "const float SEA_FREQ       = 0.16;\n"
      "const vec3 SEA_BASE        = vec3(0.1, 0.19, 0.22);\n"
      "const vec3 SEA_WATER_COLOR = vec3(0.8, 0.9, 0.6);\n"
      "#define SEA_TIME (1.0 + iTime * SEA_SPEED)\n"
      "const mat2 octave_m = mat2(1.6, 1.2, -1.2, 1.6);\n"
      "\n"
      "// math\n"
      "mat3 fromEuler(vec3 ang)\n"
      "{\n"
      "  vec2 a1 = vec2(sin(ang.x), cos(ang.x));\n"
      "  vec2 a2 = vec2(sin(ang.y), cos(ang.y));\n"
      "  vec2 a3 = vec2(sin(ang.z), cos(ang.z));\n"
      "  mat3 m;\n"
      "  m[0] = vec3(a1.y * a3.y + a1.x * a2.x * a3.x,\n"
      "              a1.y * a2.x * a3.x + a3.y * a1.x, -a2.y * a3.x);\n"
      "  m[1] = vec3(-a2.y * a1.x, a1.y * a2.y, a2.x);\n"
      "  m[2] = vec3(a3.y * a1.x * a2.x + a1.y * a3.x,\n"
      "              a1.x * a3.x - a1.y * a3.y * a2.x, a2.y * a3.y);\n"
      "  return m;\n"
      "}\n"
      "float hash(vec2 p)\n"
      "{\n"
      "  float h = dot(p, vec2(127.1, 311.7));\n"
      "  return fract(sin(h) * 43758.5453123);\n"
      "}\n"
      "float noise(in vec2 p)\n"
      "{\n"
      "  vec2 i = floor(p);\n"
      "  vec2 f = fract(p);\n"
      "  vec2 u = f * f * (3.0 - 2.0 * f);\n"
      "  return -1.0\n"
      "         + 2.0 * mix(mix(hash(i + vec2(0.0, 0.0)),\n"
      "                         hash(i + vec2(1.0, 0.0)), u.x),\n"
      "                     mix(hash(i + vec2(0.0, 1.0)),\n"
      "                         hash(i + vec2(1.0, 1.0)), u.x),\n"
      "                     u.y);\n"
      "}\n"
      "\n"
      "// lighting\n"
      "float diffuse(vec3 n, vec3 l, float p)\n"
      "{\n"
      "  return pow(dot(n, l) * 0.4 + 0.6, p);\n"
      "}\n"
      "float specular(vec3 n, vec3 l, vec3 e, float s)\n"
      "{\n"
      "  float nrm = (s + 8.0) / (PI * 8.0);\n"
      "  return pow(max(dot(reflect(e, n), l), 0.0), s) * nrm;\n"
      "}\n"
      "\n"
      "// sky\n"
      "vec3 getSkyColor(vec3 e)\n"
      "{\n"
      "  e.y = max(e.y, 0.0);\n"
      "  return vec3(pow(1.0 - e.y, 2.0),\n"
      "              1.0 - e.y,\n"
      "              0.6 + (1.0 - e.y) * 0.4);\n"
      "}\n"
      "\n"
      "// sea\n"
      "float sea_octave(vec2 uv, float choppy)\n"
      "{\n"
      "  uv += noise(uv);\n"
      "  vec2 wv  = 1.0 - abs(sin(uv));\n"
      "  vec2 swv = abs(cos(uv));\n"
      "  wv       = mix(wv, swv, wv);\n"
      "  return pow(1.0 - pow(wv.x * wv.y, 0.65), choppy);\n"
      "}\n"
      "\n"
      "float map(vec3 p)\n"
      "{\n"
      "  float freq   = SEA_FREQ;\n"
      "  float amp    = SEA_HEIGHT;\n"
      "  float choppy = SEA_CHOPPY;\n"
      "  vec2 uv      = p.xz;\n"
      "  uv.x *= 0.75;\n"
      "\n"
      "  float d, h = 0.0;\n"
      "  for (int i = 0; i < ITER_GEOMETRY; i++) {\n"
      "    d = sea_octave((uv + SEA_TIME) * freq, choppy);\n"
      "    d += sea_octave((uv - SEA_TIME) * freq, choppy);\n"
      "    h += d * amp;\n"
      "    uv *= octave_m;\n"
      "    freq *= 1.9;\n"
      "    amp *= 0.22;\n"
      "    choppy = mix(choppy, 1.0, 0.2);\n"
      "  }\n"
      "  return p.y - h;\n"
      "}\n"
      "\n"
      "float map_detailed(vec3 p)\n"
      "{\n"
      "  float freq   = SEA_FREQ;\n"
      "  float amp    = SEA_HEIGHT;\n"
      "  float choppy = SEA_CHOPPY;\n"
      "  vec2 uv      = p.xz;\n"
      "  uv.x *= 0.75;\n"
      "\n"
      "  float d, h = 0.0;\n"
      "  for (int i = 0; i < ITER_FRAGMENT; i++) {\n"
      "    d = sea_octave((uv + SEA_TIME) * freq, choppy);\n"
      "    d += sea_octave((uv - SEA_TIME) * freq, choppy);\n"
      "    h += d * amp;\n"
      "    uv *= octave_m;\n"
      "    freq *= 1.9;\n"
      "    amp *= 0.22;\n"
      "    choppy = mix(choppy, 1.0, 0.2);\n"
      "  }\n"
      "  return p.y - h;\n"
      "}\n"
      "\n"
      "vec3 getSeaColor(vec3 p, vec3 n, vec3 l, vec3 eye, vec3 dist)\n"
      "{\n"
      "  float fresnel = clamp(1.0 - dot(n, -eye), 0.0, 1.0);\n"
      "  fresnel       = pow(fresnel, 3.0) * 0.65;\n"
      "\n"
      "  vec3 reflected = getSkyColor(reflect(eye, n));\n"
      "  vec3 refracted\n"
      "    = SEA_BASE + diffuse(n, l, 80.0) * SEA_WATER_COLOR * 0.12;\n"
      "\n"
      "  vec3 color = mix(refracted, reflected, fresnel);\n"
      "\n"
      "  float atten = max(1.0 - dot(dist, dist) * 0.001, 0.0);\n"
      "  color += SEA_WATER_COLOR * (p.y - SEA_HEIGHT) * 0.18 * atten;\n"
      "\n"
      "  color += vec3(specular(n, l, eye, 60.0));\n"
      "\n"
      "  return color;\n"
      "}\n"
      "\n"
      "// tracing\n"
      "vec3 getNormal(vec3 p, float eps)\n"
      "{\n"
      "  vec3 n;\n"
      "  n.y = map_detailed(p);\n"
      "  n.x = map_detailed(vec3(p.x + eps, p.y, p.z)) - n.y;\n"
      "  n.z = map_detailed(vec3(p.x, p.y, p.z + eps)) - n.y;\n"
      "  n.y = eps;\n"
      "  return normalize(n);\n"
      "}\n"
      "\n"
      "float heightMapTracing(vec3 ori, vec3 dir, out vec3 p)\n"
      "{\n"
      "  float tm = 0.0;\n"
      "  float tx = 1000.0;\n"
      "  float hx = map(ori + dir * tx);\n"
      "  if (hx > 0.0)\n"
      "    return tx;\n"
      "  float hm   = map(ori + dir * tm);\n"
      "  float tmid = 0.0;\n"
      "  for (int i = 0; i < NUM_STEPS; i++) {\n"
      "    tmid       = mix(tm, tx, hm / (hm - hx));\n"
      "    p          = ori + dir * tmid;\n"
      "    float hmid = map(p);\n"
      "    if (hmid < 0.0) {\n"
      "      tx = tmid;\n"
      "      hx = hmid;\n"
      "    }\n"
      "    else {\n"
      "      tm = tmid;\n"
      "      hm = hmid;\n"
      "    }\n"
      "  }\n"
      "  return tmid;\n"
      "}\n"
      "\n"
      "// main\n"
      "void main(void)\n"
      "{\n"
      "  vec2 uv = -1.0 + 2.0 * vUV.xy;\n"
      "  uv      = uv * 2.0 - 1.0;\n"
      "  uv.x *= iResolution.x / iResolution.y;\n"
      "  vec2 iMouse = vec2(0.0, 0.0);\n"
      "  float time  = iTime * 0.3 + iMouse.x * 0.01;\n"
      "\n"
      "  // ray\n"
      "  vec3 ang = vec3(sin(time * 3.0) * 0.1, sin(time) * 0.2 + 0.3, time);\n"
      "  vec3 ori = vec3(0.0, 3.5, time * 5.0);\n"
      "  vec3 dir = normalize(vec3(uv.xy, -2.0));\n"
      "  dir.z += length(uv) * 0.15;\n"
      "  dir = normalize(dir) * fromEuler(ang);\n"
      "\n"
      "  // tracing\n"
      "  vec3 p;\n"
      "  heightMapTracing(ori, dir, p);\n"
      "  vec3 dist  = p - ori;\n"
      "  vec3 n     = getNormal(p, dot(dist, dist) * EPSILON_NRM);\n"
      "  vec3 light = normalize(vec3(0.0, 1.0, 0.8));\n"
      "\n"
      "  // color\n"
      "  vec3 color = mix(getSkyColor(dir),\n"
      "                   getSeaColor(p, n, light, dir, dist),\n"
      "                   pow(smoothstep(0.0, -0.05, dir.y), 0.3));\n"
      "\n"
      "  // post\n"
      "  gl_FragColor = vec4(pow(color, vec3(0.75)), 1.0);\n"
      "}\n";

public:
  ShaderMaterialSeascapeScene(ICanvas* iCanvas);
  ~ShaderMaterialSeascapeScene() override;

  const char* getName() override;
  void initializeScene(ICanvas* canvas, Scene* scene) override;

private:
  float _time;
  ShaderMaterialPtr _shaderMaterial;

}; // end of class ShaderMaterialSeascapeScene

} // end of namespace Samples
} // end of namespace BABYLON

#endif // end of
       // BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_SEASCAPE_SCENE_H
