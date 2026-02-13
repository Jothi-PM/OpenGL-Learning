#ifndef SHADERS_H
#define SHADERS_H
// Vertex Shader code
static const char* vShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 vUV;

void main() {
    vUV = aUV;
    gl_Position = vec4(aPos, 1.0);
})";

// Gray Fragment Shader
static const char* fGrayShader = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    vec4 color = texture(uTexture, vec2(1-vUV.x, vUV.y)); // mirror effect
    float gray = dot(color.rgb, vec3(0.299,0.587,0.114));
    FragColor =  vec4(vec3(gray),1.0);
}
)";

// pass through fragment shader
static const char* fPassThroughShader = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    vec4 color = texture(uTexture, vec2(1-vUV.x, vUV.y) ); // mirror effect
    FragColor = color;
}
)";

// sepia fragment shader
static const char* fSepiaShader = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    vec4 color = texture(uTexture, vec2(1-vUV.x, vUV.y) ); // mirror effect
    float r = dot(color.rgb, vec3(0.393, 0.769, 0.189));
    float g = dot(color.rgb, vec3(0.349, 0.686, 0.168));
    float b = dot(color.rgb, vec3(0.272, 0.534, 0.131));
    FragColor = vec4(r, g, b, 1.0);
}
)";

//edge detection fragment shader
static const char* fEdgeShader = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    float offset = 1.0 / 300.0; // adjust based on texture size
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(uTexture, vUV.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}
)";
#endif