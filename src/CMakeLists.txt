
aux_source_directory(. KIT_RPC_SRC)
aux_source_directory(./log KIT_LOG_SRC)
aux_source_directory(./zookeeper KIT_ZOO_SRC)
list(APPEND KIT_RPC_SRC ${KIT_LOG_SRC} ${KIT_ZOO_SRC})

aux_source_directory(${PROJECT_SOURCE_DIR}/api/proto/common/gen/ KIT_PROTO_SRC)

add_library(kit_rpc ${KIT_RPC_SRC} ${KIT_PROTO_SRC})
target_link_libraries(kit_rpc ${MUDUO_LIB} zookeeper_mt)

redefine_file_macro(kit_rpc)   #重定义 __FILE__
