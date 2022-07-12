struct Light
{
    bool enabled;
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightShadeResult
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	vec4 tint;
	float shininess;
};

uniform Material uMaterial;

in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;
in vec4 vLightSpace;

out vec4 oColor;

layout(std140, binding = 1) uniform uRendering
{
                            //base alignment    //aligned offset
    Light light;            //4                 //0
                            //16                //16
                            //16                //32
                            //16                //48
                            //16                //64
    bool toonShading;       //4                 //80

    int stepAmount;         //4                 //84
    float stepSize;         //4                 //88

    float specSize;         //4                 //92

    bool shadow;            //4                 //96
    int shadowPCF;          //4                 //100

    vec3 cameraPos;         //16                //112
};

layout(binding = 0) uniform sampler2D uTexture;
layout(binding = 1) uniform sampler2D uShadowMap;

/**
 * Get the L vector (from the light position to the vertex position)
 * @param l : the light
 */
vec3 GetLightDir(Light l)
{
    vec3 lightDir;

    // Point light
    if (l.position.w > 0.0)
    {
        vec3 lightPosFromVertexPos = (l.position.xyz / l.position.w) - vPos;
        lightDir = normalize(lightPosFromVertexPos);
        float dist = length(lightPosFromVertexPos);
    }
    // Directional light
    else
    {
        lightDir = normalize(l.position.xyz);
    }

    return lightDir;
}

/**
 * Use the shadow map to check if the vertex is lit by the light
 * @param lightSpace : the light space processed in the vertex shader
 * @param bias : the shadow bias
 * @param pcf : the shadow's "smoothness" as pixel resolution
 */
float Enlighten(vec4 lightSpace, int pcf)
{
    vec3 perspective = lightSpace.xyz / lightSpace.w;
    perspective = perspective * 0.5 + 0.5;

    if (perspective.z > 1.0)
        return 1.0;

    float currentDepth = perspective.z;

    // is the fragment lit ?
    float lit = 0.0;

    // Percentage Closer Filtering (PCF) for smooth shadows
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for (int i = -pcf; i <= pcf; ++i)
    {
        for (int j = -pcf; j <= pcf; ++j)
        {
            float pcfDepth = texture(uShadowMap, perspective.xy + vec2(i, j) * texelSize).r;
            lit += currentDepth > pcfDepth ? 0.0 : 1.0;
        }
    }

    int totalPCF = 2 * pcf + 1;
    int sqrTotalPCF = totalPCF * totalPCF;
    return lit / sqrTotalPCF;
}

/**
 * Process the light for this vertex, apply shadows if applicable
 * @param l : the light
 * @param shininess : the material's shininess
 * @param eyePosition : the camera position (or eye)
 * @param position : the vertex position
 * @param normal : the vertex normal
 * @param toon : does the shading need to be toon shading?
 */
LightShadeResult LightShade(Light l, float shininess, vec3 eyePosition, vec3 position, vec3 normal, bool toon)
{
	LightShadeResult r = LightShadeResult(vec3(0.0), vec3(0.0), vec3(0.0));
	if (!l.enabled)
		return r;

    vec3 lightDir = GetLightDir(l);

    // Process the shadow value (weather the fragment is lit or not)
    float lit = 1.0;
    if (shadow)
        lit = Enlighten(vLightSpace, shadowPCF);

    // Process the common light vectors
    vec3 eyeDir  = normalize(eyePosition - position);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specAngle = max(dot(reflectDir, eyeDir), 0.0);

    // Phong lighting
    if (!toon)
    {
        r.ambient  = l.ambient;
        r.diffuse  = lit * l.diffuse  * max(dot(normal, lightDir), 0.0);
        r.specular = lit * l.specular * pow(specAngle, shininess / 4.0);
    }
    // Toon shading
    else
    {
        // edging the shadow
        float attenuationChange = fwidth(lit) * 0.5;
        float toonLit = smoothstep(0.5 - attenuationChange, 0.5 + attenuationChange, lit);

        // process the diffuse light with a certain amount of steps
        float diffuseIntensity = ceil(max(dot(normal, lightDir) / stepSize, 0.0));
        diffuseIntensity = diffuseIntensity / stepAmount;
        diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);

        // process the specular light
        float specChange = fwidth(specAngle);
        float specIntensity = smoothstep(1 - specSize, 1 - specSize + specChange, specAngle);

        r.ambient  = l.ambient;
        r.diffuse  = toonLit * l.diffuse  * diffuseIntensity;
        r.specular = toonLit * l.specular * specIntensity;
    }

	r.specular = clamp(r.specular, 0.0, 1.0);

	return r;
}

/**
 * Process every lights' shading (there is only one light for now)
 * @param toon : does the shading need to be toon shading?
 */
LightShadeResult GetLightsShading(bool toon)
{
    LightShadeResult lightResult = LightShadeResult(vec3(0.0), vec3(0.0), vec3(0.0));

    LightShadeResult l = LightShade(light,
                                    uMaterial.shininess,
                                    cameraPos,
                                    vPos,
                                    normalize(vNormal),
                                    toon);

    lightResult.ambient  += l.ambient;
    lightResult.diffuse  += l.diffuse;
    lightResult.specular += l.specular;

    return lightResult;
}

void main()
{
    // Compute light (phong or toon) shading
    LightShadeResult lightResult = GetLightsShading(toonShading);

    vec4 texColor = texture(uTexture, vUV);

    vec3 ambientColor  = uMaterial.ambient * lightResult.ambient * (texColor.rgb + uMaterial.tint.rgb);
    vec3 diffuseColor  = uMaterial.diffuse * lightResult.diffuse * (texColor.rgb +  uMaterial.tint.rgb);
    vec3 specularColor = uMaterial.specular * lightResult.specular;
    vec3 emissiveColor = uMaterial.emissive * (texColor.rgb + uMaterial.tint.rgb);

    // Apply light color
    oColor = vec4((ambientColor + diffuseColor + specularColor + emissiveColor), uMaterial.tint.a * texColor.a);
}