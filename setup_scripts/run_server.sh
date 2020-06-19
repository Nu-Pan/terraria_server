#!/bin/bash

# parameter
install_prefix=$1

# https://terraria.gamepedia.com/Server に書いてあるおまじない
# debian + ash の場合これ書かないとダメだった
export TERM=xterm

# サーバーを起動
$install_prefix/TerrariaServer.bin.x86_64 -h
if [ $? -gt 0 ]; then
    exit 1
fi

# Choose World: n                           // New World
# Choose size: 1                            // samll
# Choose difficulty: 1                      // Classic
# Enter world name: hello
# Enter Seed (Leave Blank For Random):
#
# Choose World: 1                           // hello
# Max players: 16
# Server port: 7777
# Automatically forward port? (y/n): y
# Server password (press enter for none):

exit 0
