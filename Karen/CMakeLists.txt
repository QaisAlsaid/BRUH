cmake_minimum_required(VERSION 3.28)
project(Karen LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
#set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Build the Box2D unit tests")
#set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Build the Box2D testbed")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(KAREN_ENABLE_ASSERT "Enables Assertion" ON)
option(KAREN_ENABLE_VERIFY "Enables Verify" ON)
option(KAREN_ENABLE_INSTRUMENTATION "Enables Profiling" ON)
option(KAREN_IS_SHARED "builds as shared lib" OFF)
option(KAREN_EMSCRIPTEN "builds for the web" OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -g -O0)
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-fPIC -Wall -g -O0)
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
endif()

set(KAREN_SRC src/Karen/Core/App.cpp src/Karen/Core/Log.cpp
    src/Platforms/Linux/LinuxWindow.cpp src/Karen/Core/Layers/Layer.cpp
    src/Karen/Core/Layers/LayerStack.cpp src/Karen/Core/Layers/GuiLayer.cpp
    src/Platforms/Linux/LinuxInput.cpp
    src/Platforms/OpenGl/OpenGlContext.cpp src/Platforms/OpenGl/OpenGlVertexBuffer.cpp
    src/Platforms/OpenGl/OpenGlIndexBuffer.cpp src/Karen/Render/API/VertexBuffer.cpp
    src/Karen/Render/API/IndexBuffer.cpp
    src/Karen/Render/API/RendererAPI.cpp src/Karen/Render/API/VertexArray.cpp
    src/Platforms/OpenGl/OpenGlVertexArray.cpp src/Karen/Render/API/Shader.cpp
    src/Platforms/OpenGl/OpenGlShader.cpp src/Karen/Core/Utils/FileLoader.cpp
    src/Karen/Render/API/Renderer.cpp src/Karen/Render/API/RenderCommands.cpp
    src/Platforms/OpenGl/OpenGlRendererAPI.cpp src/Karen/Render/OrthographicCamera.cpp
    src/Platforms/Windows/WindowsInput.cpp src/Platforms/Windows/WindowsWindow.cpp
    src/Platforms/OpenGl/OpenGlTexture.cpp src/Karen/Render/API/Texture.cpp
    src/Karen/Core/Utils/ImageLoader.cpp src/Platforms/OpenGl/OpenGlCore.cpp
    src/Karen/Render/ShaderManager.cpp src/Karen/Render/OrthographicCameraController.cpp
    src/Karen/Render/Renderer2D/Renderer2D.cpp src/Platforms/OpenGl/OpenGlRendererCapabilities.cpp
    src/Karen/Core/Instrumentor.cpp src/Karen/Render/API/RendererCapabilities.cpp
    src/Karen/Render/API/FrameBuffer.cpp src/Platforms/OpenGl/OpenGlFrameBuffer.cpp
    src/Karen/Scene/Scene.cpp src/Karen/Scene/Entity.cpp src/Karen/Scene/SceneCamera.cpp
    src/Karen/Scene/SceneSerializer.cpp src/Platforms/Linux/LinuxFileDialogs.cpp
    src/Karen/Core/AssetManager.cpp src/Karen/Core/Math/math.cpp src/Karen/Core/UUID.cpp
    src/Platforms/Windows/WindowsFileDialogs.cpp src/Platforms/OpenGl/OpenGlUtils.cpp
    

    ./src/Karen/Scripting/Lua.cpp
    include/Karen/Karen.h
  )
#set(KAREN_SRC_H src/Karen/App.h src/Karen/Log.h src/Platforms/Windows/WindowsWindow.h src/Karen/Layers/Layer.h
#    src/Karen/Layers/LayerStack.h src/Karen/Layers/GuiLayer.h src/Platforms/Windows/WindowsInput.h
#  src/Karen/Input.h src/Platforms/OpenGl/OpenGlContext.h src/Platforms/OpenGl/OpenGlVertexBuffer.h
#  src/Platforms/OpenGl/OpenGlIndexBuffer.h src/Render/API/VertexBuffer.h src/Render/API/IndexBuffer.h
#  src/Render/API/RendererAPI.h src/Render/API/VertexArray.h
#  src/Platforms/OpenGl/OpenGlVertexArray.h src/Render/API/Shader.h
#  src/Platforms/OpenGl/OpenGlShader.h src/Karen/Utils/FileLoader.h
#  src/Render/API/Renderer.h src/Render/API/RenderCommands.h
#  src/Platforms/OpenGl/OpenGlRendererAPI.h
#  src/Karen/Core.h src/Karen/Window.h src/Platforms/Linux/LinuxInput.h Platforms/Linux/LinuxWindow.h  src/Render/OrthographicCamera.h
#)
set(KAREN_PUBLIC_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include/ 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/single_include/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/yaml-cpp/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/pugixml/src
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Imguizmo
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sol2/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Lua/src

  )
 set(KAREN_INCLUDE_DIRS ${KAREN_PUBLIC_INCLUDE_DIRS} PARENT_SCOPE)
 
 set(KAREN_PRIVATE_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends
  ${OPENGL_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Box2D/include
  
  )
if(NOT KAREN_EMSCRIPTEN)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
endif()
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/pugixml)
add_subdirectory(vendor/entt)
add_subdirectory(vendor/yaml-cpp)
add_subdirectory(vendor/Box2D)
add_subdirectory(vendor/Imguizmo)
add_subdirectory(vendor/sol2)
add_subdirectory(vendor/Lua)
if(NOT KAREN_EMSCRIPTEN)
  set(KAREN_LIBS glm glfw imgui glad stb pugixml yaml-cpp box2d imguizmo Lua sol2)
else()
set(KAREN_ADDITIONAL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../emsdk/upstream/emscripten/cache/sysroot/include)
set(KAREN_LIBS glm imgui stb pugixml yaml-cpp box2d imguizmo Lua sol2)
endif()

if(KAREN_IS_SHARED)
  add_library(${PROJECT_NAME} SHARED ${KAREN_SRC} ${KAREN_SRC_H})
  target_compile_definitions(${PROJECT_NAME} PRIVATE KAREN_BUILD_SHARED KAREN_IS_SHARED)
  set(KAREN_LINK_LIBS ${KAREN_LIBS} PARENT_SCOPE)
else()
  set(KAREN_LINK_LIBS  PARENT_SCOPE)
  add_library(${PROJECT_NAME} STATIC ${KAREN_SRC} ${KAREN_SRC_H})
endif(KAREN_IS_SHARED)

if(KAREN_ENABLE_ASSERT)
  target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_ENABLE_ASSERT)
endif(KAREN_ENABLE_ASSERT)

if(KAREN_ENABLE_VERIFY)
  target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_ENABLE_VERIFY)
endif(KAREN_ENABLE_VERIFY)

if(KAREN_EMSCRIPTEN)
  target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_EMSCRIPTEN)
  set(KAREN_IS_EMSCRIPTEN ${KAREN_EMSCRIPTEN} PARENT_SCOPE)
endif()

if(KAREN_ENABLE_INSTRUMENTATION)
  target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_ENABLE_INSTRUMENTATION)
  set(KAREN_INSTRUMENTATION ${KAREN_ENABLE_INSTRUMENTATION} PARENT_SCOPE)
endif()

target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.h)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${KAREN_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${KAREN_PRIVATE_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${KAREN_PUBLIC_INCLUDE_DIRS} ${KAREN_ADDITIONAL_INCLUDE})
