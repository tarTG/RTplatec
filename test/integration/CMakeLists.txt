
add_executable(
    main
    main.cpp
    simpleRender.cpp
    inputHandler.cpp
    ShaderLoader.cpp
)

SET(SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Shader/")
ADD_DEFINITIONS(-DSHADER_PATH="\\"${SHADER_PATH}\\"")


find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
find_package(X11 REQUIRED)
find_package(AntTweakBar REQUIRED)
find_package(OpenGL REQUIRED)

MESSAGE("!!!!!!! ${GLFW_INCLUDE_DIRS}")

include_directories("${GLFW_INCLUDE_DIRS}" )
include_directories("${ANTTWEAKBAR_INCLUDE_DIR}" )

target_link_libraries(main RTplatec GL3W ${OPENGL_LIBRARIES}   ${GLFW_STATIC_LIBRARIES}      ${ANTTWEAKBAR_LIBRARY} ${LIBNOISE_LIBRARIES}  ${X11_LIBRARIES})