#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(shaders)

include_directories(thirdparty)
include_directories(thirdparty/luabridge3)
include_directories(thirdparty/json)

include_directories(thirdparty/sokol)
include_directories(thirdparty/lua)
include_directories(thirdparty/stb)
include_directories(thirdparty/soloud/include)
include_directories(thirdparty/tinyobjloader)
include_directories(thirdparty/tinygltf)
include_directories(thirdparty/tinyxml2)
include_directories(thirdparty/box2d/include/box2d)
include_directories(thirdparty/joltphysics)
include_directories(net)
include_directories(global)
include_directories(ui)

add_subdirectory(thirdparty/sokol)
add_subdirectory(thirdparty/lua)
add_subdirectory(thirdparty/stb)
add_subdirectory(thirdparty/soloud)
add_subdirectory(thirdparty/tinyobjloader)
add_subdirectory(thirdparty/tinygltf)
add_subdirectory(thirdparty/tinyxml2)
add_subdirectory(thirdparty/box2d)
add_subdirectory(thirdparty/joltphysics)

if(DEFINED NO_LUA_INIT)
    add_definitions("-DNO_LUA_INIT=${NO_LUA_INIT}")
    unset(NO_LUA_INIT CACHE)
endif()
if(DEFINED NO_CPP_INIT)
    add_definitions("-DNO_CPP_INIT=${NO_CPP_INIT}")
    unset(NO_CPP_INIT CACHE)
endif()

#add_definitions("-DPROJECT_ROOT=\"${PROJECT_ROOT}\"")

file(GLOB RENDER_SOKOL_SRC renders/sokol/*.cpp)
file(GLOB_RECURSE CORE_SRCS core/*.cpp)
file(GLOB NET_SRCS net/*.cpp)
file(GLOB Ui_Srcs ui/*.cpp)

add_library(libcurl SHARED IMPORTED GLOBAL)

set_target_properties(libcurl PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/win64/libcurl.dll
  IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/libs/win64/libcurl.lib
)

add_library(libssl STATIC IMPORTED GLOBAL)
set_target_properties(libssl PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/win64/libssl.lib
)

add_library(
    cgclient

    STATIC

    ${RENDER_SOKOL_SRC}
    ${CORE_SRCS}
    ${NET_SRCS}
    ${Ui_Srcs}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(
        cgclient 

        PUBLIC 

        "-pthread"
    )
endif()

target_link_libraries(
    cgclient
    ws2_32 libcurl libssl
    sokol lua stb tinyobjloader tinygltf tinyxml2 soloud box2d joltphysics
)

set_target_properties(
    cgclient

    PROPERTIES

    CXX_STANDARD 20
)