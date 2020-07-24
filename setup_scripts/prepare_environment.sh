#!/bin/bash

# parameter
install_prefix=$1
commands_prefix=$2

# 必要なパッケージをインストール
apt-get install -y tmux netcat clang make

# 実行ファイルに実行権限を付与
chmod +x $install_prefix/TerrariaServer.bin.x86_64
if [ $? -gt 0 ]; then
    exit 1
fi

# メンテコマンドスクリプトに実行権限を付与
chmod +x $commands_prefix/*.sh
if [ $? -gt 0 ]; then
    exit 1
fi

# 正常終了
exit 0
