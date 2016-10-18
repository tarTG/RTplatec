
add_executable(
    main
    main.cpp
    simpleRender.cpp
    inputHandler.cpp
    ShaderLoader.cpp
)

SET(SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Shader/")
ADD_DEFINITIONS(-DSHADER_PATH="\\"${SHADER_PATH}\\"")

MESSAGE("\"${SHADER_PATH}\"")

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
find_package(AntTweakBar REQUIRED)
find_package(OpenGL REQUIRED)


include_directories("${GLFW_INCLUDE_DIR}" )
include_directories("${ANTTWEAKBAR_INCLUDE_DIR}" )

target_link_libraries(main RTplatec ${OPENGL_LIBRARIES} ${LIBNOISE_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${ANTTWEAKBAR_LIBRARY} GL3W)