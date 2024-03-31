cmake_minimum_required(VERSION 3.28)
project(Karen LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(KAREN_ENABLE_ASSERT "Enables Assertion" ON)
option(KAREN_ENABLE_VERIFY "Enables Verify" ON)
option(KAREN_BUILD_SHARED "builds as shared lib" ON)

add_compile_options(-fPIC -Wall -g -O0)
if(NOT WIN32)
set(KAREN_SRC src/Karen/App.cpp src/Karen/Log.cpp src/Platforms/Linux/LinuxWindow.cpp src/Karen/Layers/Layer.cpp
  src/Karen/Layers/LayerStack.cpp src/Karen/Layers/GuiLayer.cpp src/Platforms/Linux/LinuxInput.cpp
  src/Karen/Input.cpp src/Platforms/OpenGl/OpenGlContext.cpp src/Platforms/OpenGl/OpenGlVertexBuffer.cpp
  src/Platforms/OpenGl/OpenGlIndexBuffer.cpp src/Render/API/VertexBuffer.cpp src/Render/API/IndexBuffer.cpp
  src/Render/API/RendererAPI.cpp src/Render/API/VertexArray.cpp
  src/Platforms/OpenGl/OpenGlVertexArray.cpp src/Render/API/Shader.cpp
  src/Platforms/OpenGl/OpenGlShader.cpp src/Karen/CommanUtils/FileLoader.cpp
  src/Render/API/Renderer.cpp src/Render/API/RenderCommands.cpp
  src/Platforms/OpenGl/OpenGlRendererAPI.cpp
  )
else()
  set(KAREN_SRC src/Karen/App.cpp src/Karen/Log.cpp src/Platforms/Windows/WindowsWindow.cpp src/Karen/Layers/Layer.cpp
    src/Karen/Layers/LayerStack.cpp src/Karen/Layers/GuiLayer.cpp src/Platforms/Windows/WindowsInput.cpp
  src/Karen/Input.cpp src/Platforms/OpenGl/OpenGlContext.cpp src/Platforms/OpenGl/OpenGlVertexBuffer.cpp
  src/Platforms/OpenGl/OpenGlIndexBuffer.cpp src/Render/API/VertexBuffer.cpp src/Render/API/IndexBuffer.cpp
  src/Render/API/RendererAPI.cpp src/Render/API/VertexArray.cpp
  src/Platforms/OpenGl/OpenGlVertexArray.cpp src/Render/API/Shader.cpp
  src/Platforms/OpenGl/OpenGlShader.cpp src/Karen/CommanUtils/FileLoader.cpp
  src/Render/API/Renderer.cpp src/Render/API/RenderCommands.cpp
  src/Platforms/OpenGl/OpenGlRendererAPI.cpp
  )
endif()
set(KAREN_PUBLIC_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include/ 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  )
 set(KAREN_INCLUDE_DIRS ${KAREN_PUBLIC_INCLUDE_DIRS} PARENT_SCOPE)
 
 set(KAREN_PRIVATE_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends
  ${OPENGL_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include 
   )


add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)
if(KAREN_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${KAREN_SRC})
  target_compile_definitions(${PROJECT_NAME} PRIVATE KAREN_BUILD_SHARED)
endif(KAREN_BUILD_SHARED)

if(KAREN_ENABLE_ASSERT)
  target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_ENABLE_ASSERT)
endif(KAREN_ENABLE_ASSERT)

if(KAREN_ENABLE_VERIFY)
  target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_ENABLE_VERIFY)
endif(KAREN_ENABLE_VERIFY)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.h)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glm glfw imgui glad)

target_include_directories(${PROJECT_NAME} PRIVATE ${KAREN_PRIVATE_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${KAREN_PUBLIC_INCLUDE_DIRS})
