#pragma once

namespace Helpers
{
#ifdef IN_BUILD_DIRECTORY
	static constexpr char gameDirectoryPath[] = "../../game/";
	static constexpr char engineDirectoryPath[] = "../../engine/";
	static constexpr char editorDirectoryPath[] = "../../editor/";
	static constexpr char buildDirectoryPath[] = "../build/";
#else
	static constexpr char gameDirectoryPath[] = "game/";
	static constexpr char engineDirectoryPath[] = "engine/";
	static constexpr char editorDirectoryPath[] = "editor/";
	static constexpr char buildDirectoryPath[] = "build/";
#endif
}