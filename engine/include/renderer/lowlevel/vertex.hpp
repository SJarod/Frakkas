#pragma once

#include "maths.hpp"

namespace Renderer
{
	namespace LowLevel
	{
		struct Vertex
		{
		public:
			Vector3 position;
			Vector3 normal;
			Vector2 uv;

			// Used in skinned mesh
			int		boneIndices[4] = { -1, -1, -1, -1 };
			Vector4 boneWeights;
		};
	}
}