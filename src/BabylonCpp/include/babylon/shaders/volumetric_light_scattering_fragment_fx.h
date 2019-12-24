﻿#ifndef BABYLON_SHADERS_VOLUMETRIC_LIGHT_SCATTERING_FRAGMENT_FX_H
#define BABYLON_SHADERS_VOLUMETRIC_LIGHT_SCATTERING_FRAGMENT_FX_H

namespace BABYLON {

extern const char* volumetricLightScatteringPixelShader;

const char* volumetricLightScatteringPixelShader
  = R"ShaderCode(

uniform sampler2D textureSampler;
uniform sampler2D lightScatteringSampler;

uniform float decay;
uniform float exposure;
uniform float weight;
uniform float density;
uniform vec2 meshPositionOnScreen;

varying vec2 vUV;

void main(void) {
    vec2 tc = vUV;
    vec2 deltaTexCoord = (tc - meshPositionOnScreen.xy);
    deltaTexCoord *= 1.0 / float(NUM_SAMPLES) * density;

    float illuminationDecay = 1.0;

    vec4 color = texture2D(lightScatteringSampler, tc) * 0.4;

    for(int i=0; i < NUM_SAMPLES; i++) {
        tc -= deltaTexCoord;
        vec4 dataSample = texture2D(lightScatteringSampler, tc) * 0.4;
        dataSample *= illuminationDecay * weight;
        color += dataSample;
        illuminationDecay *= decay;
    }

    vec4 realColor = texture2D(textureSampler, vUV);
    gl_FragColor = ((vec4((vec3(color.r, color.g, color.b) * exposure), 1)) + (realColor * (1.5 - 0.4)));
}

)ShaderCode";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_VOLUMETRIC_LIGHT_SCATTERING_FRAGMENT_FX_H
