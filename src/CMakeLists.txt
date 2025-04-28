
aux_source_directory(. KIT_RPC_SRC)

add_library(kit_rpc SHARED ${KIT_RPC_SRC})
target_link_libraries(kit_rpc ${MUDUO_LIB})
redefine_file_macro(kit_rpc)   #重定义 __FILE__
