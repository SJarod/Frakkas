project(Editor
		LANGUAGES CXX C
		VERSION 1.0
		DESCRIPTION "Editor"
)

add_executable(${PROJECT_NAME}
			   ${CMAKE_SOURCE_DIR}/editor.rc
               src/main.cpp
			   src/editor/editor_render.cpp
			   src/editor/hierarchy.cpp
			   src/editor/menu_bar.cpp
			   src/editor/file_browser.cpp
			   src/editor/console.cpp
			   src/editor/inspector.cpp
			   src/editor/scene.cpp
			   src/editor/game_scene.cpp
			   src/editor/debugger.cpp
			   src/editor/resources_viewer.cpp
               src/helpers/game_edit.cpp
)

target_link_libraries(${PROJECT_NAME}
					  PUBLIC Game
					  PRIVATE imguizmo
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

set_target_properties(${PROJECT_NAME} PROPERTIES
					  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug
					  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/release
					  VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(copy_editor_assets

				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${CMAKE_SOURCE_DIR}/editor/assets
		 		   ${CMAKE_SOURCE_DIR}/build/debug/editor/assets
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${CMAKE_SOURCE_DIR}/imgui.ini
				   ${CMAKE_SOURCE_DIR}/build/debug

				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${CMAKE_SOURCE_DIR}/editor/assets
				   ${CMAKE_SOURCE_DIR}/build/release/editor/assets
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${CMAKE_SOURCE_DIR}/imgui.ini
				   ${CMAKE_SOURCE_DIR}/build/release
)

add_dependencies(${PROJECT_NAME} copy_editor_assets)
