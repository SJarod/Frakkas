layout(location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform uProjView
{
						//base alignment	//aligned offset
	mat4 projection;	//16				//0
						//16				//16
						//16				//32
						//16				//48
	mat4 view;			//16				//64
						//16				//80
						//16				//96
						//16				//112
};

uniform mat4 uModel;
uniform float uSize;

void main()
{
	mat4 pvm = projection * view * uModel;
	gl_Position = pvm * vec4(aPos, 1.0);
	gl_PointSize = uSize;
}