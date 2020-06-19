#!/bin/bash

# parameter
server_home=$1
world_data_dir=$2

# debug dump
echo server_home : $server_home
echo world_data_dir : $world_data_dir

# https://terraria.gamepedia.com/Server に書いてあるおまじない
# debian + bash の場合これ書かないとダメだった
export TERM=xterm

# サーバーを起動
$server_home/TerrariaServer.bin.x86_64 -h
if [ $? -gt 0 ]; then
    exit 1
fi

exit 0
