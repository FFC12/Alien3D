cmake_minimum_required(VERSION 3.15)

project(Alien3d VERSION 1.0.0 DESCRIPTION "Project description" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Git)

if (GIT_FOUND)
    if (NOT EXISTS "${PROJECT_SOURCE_DIR}/.git")
        message(STATUS "Initializing git repository...")
        execute_process(COMMAND ${GIT_EXECUTABLE} init WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} RESULT_VARIABLE GIT_INIT_RESULT)

        if (NOT GIT_INIT_RESULT EQUAL "0")
            message(FATAL_ERROR "Unable to initialize git repository.")
        endif ()
    endif ()

    message(STATUS "Retrieving git submodules...2")

    set(SUBMODULES lib/glm;lib/imgui/;lib/freetype;lib/glad;lib/glfw;lib/stb;lib/assimp;lib/box2d;)
    set(REPOSITORIES https://github.com/g-truc/glm.git;https://github.com/ocornut/imgui.git;https://github.com/aseprite/freetype2.git;https://github.com/Dav1dde/glad.git;https://github.com/glfw/glfw.git;https://github.com/nothings/stb.git;https://github.com/assimp/assimp;https://github.com/erincatto/box2d.git;)
    set(RELEASES 0.9.9.8;docking;master;master;3.3.7;master;master;main;)

    foreach (UPD_SUB IN LISTS SUBMODULES)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote -- ${UPD_SUB} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)

        list(FIND SUBMODULES ${UPD_SUB} SUB_INDEX)
        list(GET REPOSITORIES ${SUB_INDEX} SUB_URL)
        list(GET RELEASES ${SUB_INDEX} SUB_RELEASE)

        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule add ${SUB_URL} ${UPD_SUB} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${UPD_SUB} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)

            if (NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "Unable to retrieve submodule ${UPD_SUB}")
            endif ()
        endif ()

        execute_process(COMMAND ${GIT_EXECUTABLE} checkout ${SUB_RELEASE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/${UPD_SUB} RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(SEND_ERROR "Unable to checkout branch ${SUB_RELEASE} of repository ${UPD_SUB}")
        endif ()
    endforeach ()
else ()
    message(FATAL_ERROR "Git not found.")
endif ()

set(LIB_GLM ON)
set(LIB_IMGUI ON)
set(LIB_FREETYPE ON)
set(LIB_GLAD ON)
set(LIB_GLFW ON)
set(LIB_STB ON)

add_compile_definitions(LIB_GLM)
add_compile_definitions(LIB_IMGUI)
add_compile_definitions(LIB_FREETYPE)
add_compile_definitions(LIB_GLAD)
add_compile_definitions(LIB_GLFW)
add_compile_definitions(LIB_STB)

if (UNIX)
    add_compile_definitions(UNIX)
elseif (WIN32)
    add_compile_definitions(WINDOWS)
else ()
    message(FATAL_ERROR "Detected platform is not supported!")
endif ()

include_directories(lib/glm/glm)
if (NOT (LIB_GLEW OR LIB_GLAD))
    message(FATAL_ERROR "Imgui requires GLEW or GLAD.")
endif ()

if (NOT (LIB_GLFW OR LIB_SDL))
    message(FATAL_ERROR "Imgui requires GLFW or SDL.")
endif ()

include_directories(lib/imgui lib/imgui/backends)

if (LIB_GLFW)
    file(GLOB IMGUI_FILES "./lib/imgui/*.h" "./lib/imgui/*.cpp" "./lib/imgui/backends/*glfw*" "./lib/imgui/backends/*opengl3*")
elseif (LIB_SDL)
    file(GLOB IMGUI_FILES "./lib/imgui/*.h" "./lib/imgui/*.cpp" "./lib/imgui/backends/*sdl*" "./lib/imgui/backends/*opengl3*")
endif ()
add_subdirectory(lib/freetype)
include_directories(lib/freetype/include)
if (LIB_GLEW)
    message(FATAL_ERROR "GLAD is not compatible with GLEW.")
endif ()

if (NOT (LIB_GLFW OR LIB_SDL))
    message(FATAL_ERROR "GLAD requires GLFW or SDL.")
endif ()

add_subdirectory(lib/glad)
include_directories(${CMAKE_BINARY_DIR}lib/glad/include)
if (LIB_SDL)
    message(FATAL_ERROR "GLFW is not compatible with SDL.")
endif ()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
include_directories(lib/stb)

if (EXISTS ${PROJECT_SOURCE_DIR}/res/)
    message(STATUS "Copying resources...")
    file(COPY ${PROJECT_SOURCE_DIR}/res/ DESTINATION ${CMAKE_BINARY_DIR}/res/)
endif ()

message(STATUS "Setting up build options...")
file(GLOB_RECURSE SRC_FILES "./src/*.h" "./src/*.cpp")
add_executable(Alien3d ${SRC_FILES} ${IMGUI_FILES} src/gfx/GfxBase.cpp src/gfx/GfxBase.h src/adapter/GLFWImguiAdapter.h src/gfx/Buffer.h src/gfx/Shader.cpp src/gfx/Shader.h src/gfx/Texture.cpp src/gfx/Texture.h src/engine/Component.cpp src/engine/Component.h src/engine/GameObject.cpp src/engine/GameObject.h src/gfx/WorldCamera.h src/engine/Transform.h src/engine/RenderQueue.h src/engine/AssetImporter.h src/engine/Model.h src/engine/Mesh.h src/engine/Sprite.cpp src/engine/Sprite.h src/engine/SpriteBatcher.h src/gfx/Image.cpp src/gfx/Image.h src/engine/Physics2D.cpp src/engine/Physics2D.h src/engine/PhysicsBody.h src/engine/SpriteAnimation.h)

if (LIB_GLAD AND UNIX)
    set(CMAKE_CXX_FLAGS -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
endif ()

message(STATUS "Linking...")
find_package(OpenGL REQUIRED)
target_link_libraries(Alien3d ${OPENGL_LIBRARIES})

target_link_libraries(Alien3d freetype)
target_link_libraries(Alien3d glad ${CMAKE_DL_LIBS})

target_link_libraries(Alien3d glfw)

find_package(SDL2 REQUIRED)
target_link_libraries(Alien3d SDL2)

include(FindPkgConfig)
pkg_check_modules(SDL2_Mixer REQUIRED IMPORTED_TARGET SDL2_mixer)
#find_package(SDL2_mixer REQUIRED)
target_link_libraries(Alien3d SDL2_mixer)

if (UNIX)
    target_link_libraries(Alien3d X11)
endif ()

include_directories(src)

# assimp
add_subdirectory(lib/assimp)
set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS ON)
set(ASSIMP_BUILD_STATIC_LIB ON)
set(ASSIMP_BUILD_NO_SMD_IMPORTER ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
include_directories(${CMAKE_BINARY_DIR}/lib/assimp/include)
target_link_libraries(Alien3d assimp)

# sol3
find_package(Lua REQUIRED)
include_directories(/usr/local/include ${LUA_INCLUDE_DIR})

target_link_libraries(Alien3d ${LUA_LIBRARIES})
include_directories(${CMAKE_BINARY_DIR}/lib/sol3/sol)

# box2d
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(lib/box2d)
include_directories(${CMAKE_BINARY_DIR}}/lib/box2d/include)
target_link_libraries(Alien3d box2d)
