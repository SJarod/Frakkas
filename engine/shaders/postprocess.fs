out vec4 oColor;

in vec2 vUV;

layout(binding = 0) uniform sampler2D uRenderTex;
layout(binding = 1) uniform sampler2D uDepthStencilTex;

//uniform float uOffset;
//uniform mat3 uKernel;

void main()
{
    float uOffset = 0.002;
    mat3 uKernel;
    uKernel[0] = vec3(1.0, 1.0, 1.0);
    uKernel[1] = vec3(1.0, -8.0, 1.0);
    uKernel[2] = vec3(1.0, 1.0, 1.0);

    vec2 offsets[9] = vec2[](
        vec2(-uOffset,  uOffset),
        vec2( 0.0,     uOffset),
        vec2( uOffset,  uOffset),
        vec2(-uOffset,  0.0),
        vec2( 0.0,     0.0),
        vec2( uOffset,  0.0),
        vec2(-uOffset, -uOffset),
        vec2( 0.0,    -uOffset),
        vec2( uOffset, -uOffset)
    );

    vec3 surroundingFrags[9];
    for (int i = 0; i < 9; ++i)
    {
        vec3 frag = vec3(texture(uDepthStencilTex, vUV + offsets[i])) + 
                    vec3(texture(uRenderTex, vUV + offsets[i]));

        surroundingFrags[i] = frag;
    }

    vec3 blendedColor = vec3(0.0);
    for (int i = 0; i < 9; ++i)
    {
        blendedColor += surroundingFrags[i] * uKernel[i / 3][i % 3];
    }
    
    float rgb = blendedColor.x + blendedColor.y + blendedColor.z;
    oColor = vec4(vec3(rgb), 1.0);
}