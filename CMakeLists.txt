cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(gamelift-pb)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(com_github_protocolbuffers_protobuf
    URL https://github.com/protocolbuffers/protobuf/archive/v3.3.2.zip
    URL_HASH SHA256=c895ad9fd792532f233ced36969d9cc4daec5cb7de9db0d9b26cf06ccd0183c1
)
FetchContent_GetProperties(com_github_protocolbuffers_protobuf)
set(protobuf_BUILD_TESTS OFF)
if(NOT com_github_protocolbuffers_protobuf_POPULATED)
    FetchContent_Populate(com_github_protocolbuffers_protobuf)

    add_subdirectory(${com_github_protocolbuffers_protobuf_SOURCE_DIR}/cmake EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(com_amazonaws_gamelift
    URL https://gamelift-release.s3-us-west-2.amazonaws.com/GameLift_04_16_2020.zip
    URL_HASH SHA256=5f55d510218d755e37b70eb7aa30f8f7a3554a8e5f8ec5f2f789cd6a5536f348
)
FetchContent_GetProperties(com_amazonaws_gamelift)
if(NOT com_amazonaws_gamelift_POPULATED)
    FetchContent_Populate(com_amazonaws_gamelift)
endif()
set(GAMELIFT_SOURCE_DIR ${com_amazonaws_gamelift_SOURCE_DIR} CACHE FILEPATH "Filepath to GameLift Server SDK zip file contents.")

if(NOT napi_build_version)
    set(napi_build_version 6)
endif()

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

add_library(${PROJECT_NAME}-wire-test SHARED EXCLUDE_FROM_ALL)

target_compile_definitions(${PROJECT_NAME}-wire-test PUBLIC
    NAPI_VERSION=${napi_build_version}
    GAMELIFT_USE_STD
    __STDC_LIB_EXT1__
    __STDC_WANT_LIB_EXT1__=1
)

set_target_properties(${PROJECT_NAME}-wire-test PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME}-wire-test PROPERTIES OUTPUT_NAME "wire-test")
set_target_properties(${PROJECT_NAME}-wire-test PROPERTIES SUFFIX ".node")

if(CMAKE_JS_VERSION)
    target_include_directories(${PROJECT_NAME}-wire-test PUBLIC ${CMAKE_JS_INC})
else()
    # TODO: This won't build on windows
    target_include_directories(${PROJECT_NAME}-wire-test PUBLIC /usr/include/node)
endif()

target_include_directories(${PROJECT_NAME}-wire-test PRIVATE
    ${NODE_ADDON_API_DIR}
    ${GAMELIFT_SOURCE_DIR}/GameLift-SDK-Release-4.0.0/GameLift-Cpp-ServerSDK-3.4.0/gamelift-server-sdk/source/aws/gamelift/server/protocols
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/include
)
target_link_libraries(${PROJECT_NAME}-wire-test PRIVATE
    libprotobuf
)
target_sources(${PROJECT_NAME}-wire-test PRIVATE
    ${GAMELIFT_SOURCE_DIR}/GameLift-SDK-Release-4.0.0/GameLift-Cpp-ServerSDK-3.4.0/gamelift-server-sdk/source/aws/gamelift/server/protocols/sdk.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/wire-test.cc
)

enable_testing()
add_test(NAME wire-test COMMAND ${PROJECT_NAME}-wire-test)
