
include_directories(
    include
    ${PROJECT_SOURCE_DIR}/api/
)

set(CALLEE_SRC
    test_callee.cpp
    ${PROJECT_SOURCE_DIR}/api/proto/user/gen/user.pb.cc
)


add_executable(test_callee ${CALLEE_SRC})
target_link_directories(test_callee PRIVATE ${EXTERN_LIB_PATH})
redefine_file_macro(test_callee)   #重定义 __FILE__
target_link_libraries(test_callee ${LIB_LIB})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
