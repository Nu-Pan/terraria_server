#!/bin/bash

# 過去 wait_time_in_min 分間プレイヤー数が 0 人 の状態が続いた場合のみ exit コマンドを terraria server に送信する

# config
safe_shutdown_dir_path=/tmp/safe_shutdown
wait_time_in_min=5
delay_in_sec=1

# 初回起動時は強制的に生存扱いにする
if [ ! -d $safe_shutdown_dir_path ]; then
    mkdir -p $safe_shutdown_dir_path
    force_alive=1
else
    force_alive=0
fi
echo force_alive: $force_alive

# システム起動時間を記録
up_time_in_sec=`cat /proc/uptime | sed -r "s/^([0-9]+).*$/\1/"`
echo up_time_in_sec: $up_time_in_sec

# playing_result の結果をサーバーから取得
s2s-write << EOF
playing
EOF
sleep $delay_in_sec
playing_result_raw=`s2s-read`
echo playing_result_raw: \"$playing_result_raw\"
playing_result=`echo $playing_result_raw | grep "No players connected."`
echo playing_result: \"$playing_result\"

# プレイヤーがいる場合は生存記録を残して終了
if [ ! "$playing_result" ] || [ $force_alive -eq 1 ]; then
    echo alive
    rm -f `find $safe_shutdown_dir_path -type f -name "*"`
    touch $safe_shutdown_dir_path/$up_time_in_sec
    exit 0
else
    echo dead
fi

# 最後の生存記録時間を取得
last_time_in_sec=`ls $safe_shutdown_dir_path | head -1`
if [ ! $last_time_in_sec ]; then
    echo No last alive file in $safe_shutdown_dir_path
    exit 0
fi
echo last_time_in_sec: $last_time_in_sec

# 最後の生存記録から wait_time_in_min 経ってたら終了メッセージを送る
wait_time_in_sec=$(($wait_time_in_min * 60))
dead_time_in_sec=$(($wait_time_in_sec + $last_time_in_sec))
echo dead_time_in_sec: $dead_time_in_sec
if [ $up_time_in_sec -gt $dead_time_in_sec ]; then
echo Send exit command
s2s-write << EOF
exit
EOF
exit 0
fi

# 何もしない
echo No action

exit 0
