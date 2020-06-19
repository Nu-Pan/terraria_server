#!/bin/bash

# parameter
package_url="$1"
server_home="$2"

# alias
package_file_name="${package_url##*/}"
package_dir_name=`echo "$package_file_name" | sed -e "s/terraria-server-\([0-9]\{4\}\).zip/\1/g"`

# debug dump
echo package_url : $package_url
echo package_file_name : $package_file_name
echo package_dir_name : $package_dir_name
echo server_home : $server_home

# パッケージ展開済みの場合はスキップ
if [ -e $server_home ]; then
    exit 0
fi

# パッケージをファイルサーバーから取ってくる
wget $package_url -O $package_file_name
if [ $? -gt 0 ]; then
    exit 1
fi

# パッケージを展開
unzip $package_file_name
if [ $? -gt 0 ]; then
    exit 1
fi

# アーカイブを削除
if [ -e $package_file_name ]; then
    rm -f $package_file_name
fi

# win, mac のファイル群を削除
if [ -e $package_dir_name/Windows ]; then
    rm -rf $package_dir_name/Windows
fi
if [ -e $package_dir_name/Mac ]; then
    rm -rf $package_dir_name/Mac
fi

# linux サーバーファイル群を所定のパスに配置
if [ ! -e $server_home ]; then
    mv $package_dir_name/Linux $server_home
    rm -rf $package_dir_name
fi

# 正常終了
exit 0
