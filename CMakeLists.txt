cmake_minimum_required(VERSION 3.0)
project(rpc通信框架)


set(CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3  --std=c++17 -fPIC -ggdb -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations -Wno-sign-compare -Wno-unused-result")


# 查找gtest库
find_package(GTest REQUIRED)

# 查找absl库
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/absl")
find_package(absl REQUIRED)

#  查找protobuf库
find_package(protobuf REQUIRED)

include_directories(
    include
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/api/

)

set(EXEC_SRC
    src/kit_rpc_application.cpp
    src/rpc_provider.cpp
)

add_library(kit_rpc SHARED ${EXEC_SRC})
redefine_file_macro(kit_rpc)   #重定义 __FILE__

set(EXTERN_LIB_PATH
    "${PROJECT_SOURCE_DIR}/lib/muduo")

set(MUDUO_LIB
    muduo_base
    muduo_http
    muduo_inspect
    muduo_net
)

set(LIB_LIB
    kit_rpc
    dl
    pthread
    GTest::GTest
    GTest::Main
    ${MUDUO_LIB}
    protobuf::libprotobuf
    absl::strings
    absl::str_format
    absl::flat_hash_map
)

set(DEPENDS
    kit_rpc
)

set(SUB_DIR
    # src
    example
)

# 添加子目录路径
add_subdirectory(${SUB_DIR})

# **********************************tests**********************************#
# test_rpc 主程序测试 test_rpc
# add_executable(test_rpc tests/test_rpc.cpp)
# add_dependencies(test_rpc ${DEPENDS})
# target_link_directories(test_rpc PRIVATE ${EXTERN_LIB_PATH})
# redefine_file_macro(test_rpc)   #重定义 __FILE__
# target_link_libraries(test_rpc ${LIB_LIB})

# add_executable(test_muduo tests/test_muduo.cpp)
# add_dependencies(test_muduo ${DEPENDS})
# target_link_directories(test_muduo PRIVATE ${EXTERN_LIB_PATH})
# redefine_file_macro(test_muduo)   #重定义 __FILE__
# target_link_libraries(test_muduo ${LIB_LIB})

add_executable(test_proto tests/test_proto.cpp tests/test.pb.cc)
add_dependencies(test_proto ${DEPENDS})
target_link_directories(test_proto PRIVATE ${EXTERN_LIB_PATH})
redefine_file_macro(test_proto)   #重定义 __FILE__
target_link_libraries(test_proto ${LIB_LIB})

# **********************************example**********************************#
# Tcp Server 实例文件 echo_tcp_server
# add_executable(echo_tcp_server example/echo_tcp_server.cpp)
# add_dependencies(echo_tcp_server ${DEPENDS})
# target_link_libraries(echo_tcp_server ${LIB_LIB})



# **********************************压测例子**********************************#

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

