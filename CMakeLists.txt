cmake_minimum_required(VERSION 3.28)

file(GLOB knuxfansrc
     "source/*.cpp",
     "source/utils/*.cpp",
     "source/scenes/*.cpp",
     "source/opengl/*.cpp",
     "source/opengl/*.c",
     "source/opengl/triangle/*.cpp"



)
project(KnuxfanWaiter)

add_compile_definitions(_LINUX)
add_compile_definitions(CLIENT)

find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(app ${SDL2_INCLUDE_DIRS})
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} )
SET(CMAKE_CXX_FLAGS "-O2 -std=c++17")
include_directories("./source")
include_directories("./include")
add_executable(app ${knuxfansrc})  # be sure there's exactly one main() function in the source files
target_link_libraries(app ${SDL2_LIBRARIES})
target_link_libraries(app ${FREETYPE_LIBRARIES})
target_include_directories(app PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(app ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} )
