layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

#ifdef SKINNING
layout(location = 3) in ivec4 aBoneIndices;
layout(location = 4) in vec4 aBoneWeights;

const int MAX_BONES = 100;
uniform mat4 uBoneTransforms[MAX_BONES];
#endif

layout(std140, binding = 0) uniform uRenderMatrices
{
							//base alignment	//aligned offset
	mat4 projection;		//16				//0
							//16				//16
							//16				//32
							//16				//48
	mat4 view;				//16				//64
							//16				//80
							//16				//96
							//16				//112
};

layout(std140, binding = 2) uniform uLightMatrices
{
	mat4 lightProjection;
	mat4 lightView;
};

uniform mat4 uModel;
uniform mat4 uModelNormal;

out vec3 vPos;
out vec3 vNormal;
out vec2 vUV;
out vec4 vLightSpace;

void main()
{
	vec4 pos = vec4(aPos, 1.0);
#ifdef SKINNING
	mat4 boneMat = aBoneWeights[0] * uBoneTransforms[aBoneIndices[0]]
				 + aBoneWeights[1] * uBoneTransforms[aBoneIndices[1]]
				 + aBoneWeights[2] * uBoneTransforms[aBoneIndices[2]]
				 + aBoneWeights[3] * uBoneTransforms[aBoneIndices[3]];
	pos = boneMat * pos;
#endif
    pos = uModel * pos;
    vPos = pos.xyz / pos.w;

	vec3 normal = aNormal;
#ifdef SKINNING
	normal = mat3(uBoneTransforms[aBoneIndices[0]]) * aNormal
		   + mat3(uBoneTransforms[aBoneIndices[1]]) * aNormal
		   + mat3(uBoneTransforms[aBoneIndices[2]]) * aNormal
		   + mat3(uBoneTransforms[aBoneIndices[3]]) * aNormal;
#endif
    vNormal = (uModelNormal * vec4(normal, 0.0)).xyz;

	vUV = aUV;

	mat4 lpvm = lightProjection * lightView;
	vLightSpace = lpvm * pos;

	mat4 pvm = projection * view;
	gl_Position = pvm * pos;
}