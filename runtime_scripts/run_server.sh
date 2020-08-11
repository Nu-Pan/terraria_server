#!/bin/bash

# parameter
install_prefix=$1
commands_prefix=$2

# https://terraria.gamepedia.com/Server に書いてあるおまじない
# debian + ash の場合これ書かないとダメだった
export TERM=xterm

# サーバーを起動
coproc $install_prefix/TerrariaServer.bin.x86_64
if [ $? -gt 0 ]; then
    exit 1
fi

# netcat 
while true
do
#    nc -kl -p 7776 <&"${COPROC[0]}" >&"${COPROC[1]}"
    jobs
    echo $?
    sleep 1
done

# 正常終了
exit 0
