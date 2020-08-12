# 本番コンテナがベースとするイメージ
# bazel 上 daemon-less

# alpine は gcc ベースじゃないので terraria が動かない
FROM ubuntu:20.04

RUN apt-get update \
&&  apt-get install -y \
    cron \
&&  apt-get -y clean \
&&  rm -rf /var/lib/apt/lists/*
