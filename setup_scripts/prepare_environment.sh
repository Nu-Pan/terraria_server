#!/bin/bash

# parameter
install_prefix=$1

# 実行ファイルに実行権限を付与
chmod +x "$install_prefix/TerrariaServer.bin.x86_64"
if [ $? -gt 0 ]; then
    exit 1
fi

# 正常終了
exit 0
