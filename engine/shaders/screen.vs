layout(location = 0) in vec2 aPos;

out vec2 vUV;

void main()
{
	vUV = vec2(aPos.x < 0.f ? 0.f : aPos.x,
			   aPos.y < 0.f ? 0.f : aPos.y);
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}