cmake_minimum_required(VERSION 3.28)
project(KarenEditor)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(KAREN_EMSCRIPTEN OFF CACHE BOOL "enable build for the web")

add_subdirectory(../Karen/ ../Karen/build/)
add_executable(${PROJECT_NAME} src/App.cpp src/EditorLayer.cpp
  src/Panels/SceneHierarchy.cpp src/Panels/Inspector.cpp
  src/HelperWindows.cpp src/EditorSerializer.cpp src/Panels/ContentBrowser.cpp
  src/Panels/AssetManagerGui.cpp src/EditorCamera.cpp)
message("idrs" ${KAREN_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${KAREN_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Karen)
if(${KAREN_IS_EMSCRIPTEN})
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html"
    LINK_FLAGS "-sUSE_GLFW=3 -sWASM=0 -sFULL_ES3 --preload-file ../res --bind -DKAREN_EMSCRIPTEN --shell-file template.html"

    #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
endif()
