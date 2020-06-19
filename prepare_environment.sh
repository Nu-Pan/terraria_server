#!/bin/bash

# parameter
server_home=$1
world_data_dir=$2

# debug dump
echo server_home : $server_home
echo world_data_dir : $world_data_dir

# 実行ファイルに実行権限を付与
sudo chmod +x "$server_home/TerrariaServer.bin.x86_64"
if [ $? -gt 0 ]; then
    exit 1
fi

# ワールドデータディレクトリを作成
if [ ! -e $world_data_dir ]; then
    mkdir $world_data_dir
fi

# 正常終了
exit 0
