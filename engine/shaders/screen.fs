out vec4 oColor;

in vec2 vUV;

layout(binding = 0) uniform sampler2D uFirstPass;
layout(binding = 1) uniform sampler2D uSecondPass;

uniform bool postprocess = false;

void main()
{
	oColor = texture(uFirstPass, vUV);
	if (postprocess)
		oColor *= (vec4(1.0) - vec4(texture(uSecondPass, vUV).rgb, 0.0));
}