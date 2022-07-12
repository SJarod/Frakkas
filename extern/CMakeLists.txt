########## ASSIMP
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(USE_STATIC_CRT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)

if(MSVC)
    add_compile_options(/EHs)
endif()

add_subdirectory(assimp)

########## SDL
set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(SDL_STATIC_ENABLED_BY_DEFAULT ON CACHE BOOL "" FORCE)
set(SDL_LIBC ON CACHE BOOL "Use the system C library" FORCE)
set(SDL_FORCE_STATIC_VCRT ON CACHE BOOL "" FORCE)

add_subdirectory(sdl)

########## JOLT
add_library(jolt STATIC IMPORTED GLOBAL)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set_target_properties(jolt PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/jolt/build/Windows/Debug/Jolt.lib")
    elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
        set_target_properties(jolt PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/jolt/build/Windows/Release/Jolt.lib")
    endif()

elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set_target_properties(jolt PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/jolt/build/Linux/Debug/libJolt.a")
    elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
        set_target_properties(jolt PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/jolt/build/Linux/Release/libJolt.a")
    endif()
endif()

set_target_properties(jolt PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extern/jolt")

########## TRACY
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/extern/tracy_executable)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/extern/tracy_executable.7z
                         DESTINATION ${CMAKE_SOURCE_DIR}/extern
)
endif()

set(TRACY_ENABLE ON CACHE BOOL "" FORCE)
add_subdirectory(tracy)

########## STB
add_library(stb INTERFACE)
target_sources(stb INTERFACE "stb_image/stb_image.c")
target_include_directories(stb INTERFACE stb_image)

########## MINIAUDIO
add_library(miniaudio INTERFACE)
target_sources(miniaudio INTERFACE "miniaudio/miniaudio.c")
target_include_directories(miniaudio INTERFACE miniaudio)

########## GLAD
add_library(glad INTERFACE)
target_sources(glad INTERFACE "glad/src/glad.c")
target_include_directories(glad INTERFACE glad/include)

########## IMGUI
add_library(imgui INTERFACE)
target_sources(imgui INTERFACE
                     "imgui/imgui.cpp"
                     "imgui/imgui_demo.cpp"
                     "imgui/imgui_draw.cpp"
                     "imgui/imgui_widgets.cpp"
                     "imgui/imgui_tables.cpp"
                     "imgui/backends/imgui_impl_sdl.cpp"
                     "imgui/backends/imgui_impl_opengl3.cpp"
                     "imgui/misc/cpp/imgui_stdlib.cpp"
)
target_include_directories(imgui INTERFACE imgui)

########## IMGUIZMO
add_library(imguizmo INTERFACE)
target_sources(imguizmo INTERFACE
                        "imguizmo/ImCurveEdit.cpp"
                        "imguizmo/ImGradient.cpp"
                        "imguizmo/ImGuizmo.cpp"
                        "imguizmo/ImSequencer.cpp"
)
target_include_directories(imguizmo INTERFACE imguizmo)