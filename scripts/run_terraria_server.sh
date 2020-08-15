#!/bin/bash

# https://terraria.gamepedia.com/Server に書いてあるおまじない
# debian + ash の場合これ書かないとダメだった
export TERM=xterm

# stdio2session の状態をリセット
s2s-clean

# サーバーをバックグラウンドで起動
s2s-server /usr/bin/TerrariaServer.bin.x86_64 &
if [ $? -gt 0 ]; then
    exit 1
fi

# ワールド起動までのコマンドを流し込む
worlds=`find ~/.local/share/Terraria/Worlds/ | grep .wld`
if [ "$worlds" = "" ]; then

# ワールドが無い場合
s2s-write << EOF
n
1
1
default
114514
1
16
7777
y
hikehike
EOF

else

# ワールドがある場合
s2s-write << EOF
1
16
7777
y
hikehike
EOF

fi

# ログがでなくなるまで繰り返し読み出す
empty_count=0
while [ $empty_count -lt 10 ]
do
    sleep 1
    read_buffer=`s2s-read`
    if [ -z "$read_buffer" ]; then
        empty_count=`expr $empty_count + 1`
    else
        empty_count=0
        echo "$read_buffer"
    fi
done

# 自動処理をスタート
crontab /etc/cron.d/terraria_server
service cron restart

# サーバーの終了を待機
echo Terraria server is ready!
wait

# Google Compute Engine 環境下の場合はインスタンス停止を実行
curl http://metadata.google.internal
if [ $? -eq 0 ]; then
    gce_access_token=`curl -X GET -H "Metadata-Flavor: Google" http://metadata.google.internal/computeMetadata/v1/instance/service-accounts/default/token | jq -r '.access_token'`
    gce_project_id=`curl -X GET -H "Metadata-Flavor: Google" http://metadata.google.internal/computeMetadata/v1/project/project-id`
    gce_zone=`curl -X GET -H "Metadata-Flavor: Google" http://metadata.google.internal/computeMetadata/v1/instance/zone | sed -r "s/.+\/(.+)/\1/"`
    gce_hostname=$HOSTNAME
    curl -X POST -H "Authorization":"Bearer $gce_access_token" "https://www.googleapis.com/compute/v1/projects/$gce_project_id/zones/$gce_zone/instances/$gce_hostname/stop"
fi

# 正常終了
exit 0
