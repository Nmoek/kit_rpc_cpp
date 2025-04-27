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

link_directories(${PROJECT_SOURCE_DIR}/lib)



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


# **********************************tests**********************************#
# test_rpc 主程序测试 test_rpc
add_executable(test_rpc tests/test_rpc.cpp)
add_dependencies(test_rpc ${DEPENDS})
target_link_directories(test_rpc PRIVATE ${EXTERN_LIB_PATH})
redefine_file_macro(test_rpc)   #重定义 __FILE__
target_link_libraries(test_rpc ${LIB_LIB})

# moduo库测试
add_executable(test_muduo tests/test_muduo.cpp)
add_dependencies(test_muduo ${DEPENDS})
target_link_directories(test_muduo PRIVATE ${EXTERN_LIB_PATH})
redefine_file_macro(test_muduo)   #重定义 __FILE__
target_link_libraries(test_muduo ${LIB_LIB})

# protoc测试
add_executable(test_proto tests/test_proto.cpp tests/test.pb.cc)
add_dependencies(test_proto ${DEPENDS})
target_link_directories(test_proto PRIVATE ${EXTERN_LIB_PATH})
redefine_file_macro(test_proto)   #重定义 __FILE__
target_link_libraries(test_proto ${LIB_LIB})


# **********************************example**********************************#
# callee 发布rpc服务方 示例
add_executable(test_callee example/callee/test_callee.cpp)
target_link_directories(test_callee PRIVATE ${EXTERN_LIB_PATH})
redefine_file_macro(test_callee)   #重定义 __FILE__
target_link_libraries(test_callee ${LIB_LIB})


# 设置可执行文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 添加子目录路径
add_subdirectory(src)

