#!/bin/bash

# 配置参数
PROTO_ROOT_DIR="api/proto"          # proto根目录
PROTOC=$(which protoc)        # protoc路径

# 检查protoc是否存在
if [ ! -f "$PROTOC" ]; then
    echo "error: 未找到protoc编译器，请先安装protobuf!"
    exit 1
fi

echo "proto文件生成开始!"

# 遍历所有子目录（如a, b）
for sub_dir in $PROTO_ROOT_DIR/*/; do
    # 提取子目录名（如a或b）
    dir_name=$(basename $sub_dir)

    # 创建对应的gen目录
    gen_dir="$sub_dir/gen"
    mkdir -p $gen_dir

    # 编译该子目录下的所有proto文件
    echo "处理目录: $sub_dir"
    for proto_file in $sub_dir/*.proto; do
        if [ -f "$proto_file" ]; then
            echo "  编译: $(basename $proto_file)"
            $PROTOC --cpp_out=$gen_dir -I $sub_dir $proto_file
            if [ $? -ne 0 ]; then
                echo "错误: 编译失败: $proto_file"
                exit 1
            fi
        fi
    done
done

echo "所有proto文件编译完成!"

mkdir -p build/
cd build/
cmake ..
make -j
cd ..