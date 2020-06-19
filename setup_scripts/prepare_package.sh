#!/bin/bash

# config
required_deb_packages="wget unzip"

# parameter
package_url=$1
install_prefix=$2

# alias
package_file_name="${package_url##*/}"
package_dir_name=`echo "$package_file_name" | sed -e "s/terraria-server-\([0-9]\{4\}\).zip/\1/g"`

# 作業上必要な deb パッケージを入れる
apt-get install -y $required_deb_packages
if [ $? -gt 0 ]; then
    exit 1
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

# linux サーバーファイル群を所定のパスに配置
mv $package_dir_name/Linux $install_prefix
if [ $? -gt 0 ]; then
    exit 1
fi

# 作業上必要な deb パッケージを削除
apt-get remove -y $required_deb_packages
if [ $? -gt 0 ]; then
    exit 1
fi

# 正常終了
exit 0
