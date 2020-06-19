#!/bin/bash

# config
package_url="https://www.terraria.org/system/dedicated_servers/archives/000/000/039/original/terraria-server-1405.zip"
server_home="terraria_server"
world_data_dir="terraria_world"

# パッケージを展開
./prepare_package.sh "$package_url" "$server_home"
if [ $? -gt 0 ]; then
    echo failed to prepare terraria server package.
    exit 1
fi

# サーバー実行環境を整える
./prepare_environment.sh "$server_home" "$world_data_dir"
if [ $? -gt 0 ]; then
    echo Failed to prepare environment.
    exit 1
fi

# サーバーを実行
./run_server.sh "$server_home" "$world_data_dir"
if [ $? -gt 0 ]; then
    echo Failed to run server.
    exit 1
fi

# 正常終了
exit 0
