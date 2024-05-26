cmake_minimum_required(VERSION 3.28)
project(Script)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(KAREN_EMSCRIPTEN OFF CACHE BOOL "enable build for the web")
#add_subdirectory(../../../Karen/ ../../../Karen/build/)
add_library(${PROJECT_NAME} SHARED Test.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
add_compile_options(-fPIC)
target_compile_definitions(${PROJECT_NAME} PUBLIC KAREN_ENABLE_ASSERT)
message("idrs" ${KAREN_INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PUBLIC ${KAREN_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ../../../Karen/src/
  ../../../Karen/include/
  ../../../Karen/vendor/glm/
  ../../../Karen/vendor/entt/single_include/
  ../../../Karen/vendor/imgui/
  ../../../Karen/vendor/spdlog/include/) 
#../../Karen/build/libKaren.so
target_link_libraries(${PROJECT_NAME}  ${CMAKE_CURRENT_SOURCE_DIR}/../../Karen/build/libKaren.so) #Karen)
