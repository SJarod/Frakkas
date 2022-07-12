layout(location = 0) in vec3 aPos;

#ifdef SKINNING
layout(location = 3) in ivec4 aBoneIndices;
layout(location = 4) in vec4 aBoneWeights;

const int MAX_BONES = 100;
uniform mat4 uBoneTransforms[MAX_BONES];
#endif

uniform mat4 uModel;

layout(std140, binding = 2) uniform uLightMatrices
{
							//base alignement	//aligned offset
	mat4 lightProjection;	//16				//0
							//16				//16
							//16				//32
							//16				//48
	mat4 lightView;			//16				//64
							//16				//80
							//16				//96
							//16				//112
};

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

	mat4 lpvm = lightProjection * lightView * uModel;
	gl_Position = lpvm * pos;
}