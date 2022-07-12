project(Engine
		LANGUAGES CXX C
		VERSION 1.0
		DESCRIPTION "Engine"
)

add_library(${PROJECT_NAME} STATIC
			src/game/entity_manager.cpp
        	src/game/transform.cpp
			src/game/world.cpp
			src/game/component/drawable/static_draw.cpp
			src/game/component/drawable/animated_draw.cpp
			src/game/component/camera.cpp
			src/game/component/sound.cpp
			src/game/component/collider/box_collider.cpp
			src/game/component/collider/sphere_collider.cpp
			src/game/component/ui/text.cpp
			src/game/component/ui/button.cpp
			src/game/component/ui/panel.cpp
			src/game/component/ui/image_panel.cpp
			src/game/component/ui/image_button.cpp
			src/game/lowcomponent/ui_object.cpp
            src/game/lowcomponent/component.cpp
			src/game/lowcomponent/drawable.cpp
			src/game/lowcomponent/collider.cpp
			src/game/entity.cpp
            src/game/time_manager.cpp
            src/game/inputs_manager.cpp
			src/game/component_generator.cpp
			src/renderer/render_object.cpp
			src/renderer/model.cpp
			src/renderer/skeletal_model.cpp
			src/renderer/point.cpp
			src/renderer/screen.cpp
			src/renderer/lowlevel/lowrenderer.cpp
        	src/renderer/graph.cpp
			src/resources/program_shader.cpp
			src/resources/resources_manager.cpp
			src/resources/serializer.cpp
			src/resources/mesh.cpp
			src/resources/skeletal_mesh.cpp
			src/resources/skeletal_animation.cpp
			src/resources/texture.cpp
			src/animation/animator.cpp
			src/animation/animation_graph.cpp
			src/ui/canvas.cpp
			src/debug/log.cpp
            src/engine.cpp
        	src/multithread/threadpool.cpp
		    src/physic/physic_scene.cpp
			src/physic/contact_listener.cpp
			src/physic/layers.cpp
)

target_include_directories(${PROJECT_NAME}
							PUBLIC include
							PUBLIC include/game/component
)

target_link_libraries(${PROJECT_NAME}
					  PUBLIC glad
					  PUBLIC SDL2-static
					  PUBLIC TracyClient
					  PUBLIC assimp::assimp
					  PUBLIC jolt
					  PUBLIC miniaudio
					  PRIVATE stb
					  PUBLIC imgui
					  PUBLIC Mathlib
)

add_custom_target(copy_shaders
				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${CMAKE_SOURCE_DIR}/engine/shaders
				  ${CMAKE_SOURCE_DIR}/build/debug/engine/shaders

				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${CMAKE_SOURCE_DIR}/engine/shaders
				  ${CMAKE_SOURCE_DIR}/build/release/engine/shaders
)

add_dependencies(${PROJECT_NAME} copy_shaders)
