#!/bin/bash

# parameter
install_prefix=$1
commands_prefix=$2

# https://terraria.gamepedia.com/Server に書いてあるおまじない
# debian + ash の場合これ書かないとダメだった
export TERM=xterm

# サーバーを起動
$install_prefix/TerrariaServer.bin.x86_64
if [ $? -gt 0 ]; then
    exit 1
fi

# 正常終了
exit 0
