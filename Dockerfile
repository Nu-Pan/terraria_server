# note:
# 開発用コンテナなので容量を気にする必要はない
# なのでrun は１つにまとめない

# alpine は gcc ベースじゃないので terraria が動かない
FROM ubuntu:20.04

# 参照先リポジトリを JAIST にする
RUN sed -i.bak -e "s%http://[^ ]\+%http://ftp.jaist.ac.jp/pub/Linux/ubuntu/%g" /etc/apt/sources.list

# 先にコレをやっておかないと、タイムゾーンの入力を求められてイメージビルドが止まる
# https://qiita.com/yagince/items/deba267f789604643bab
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    tzdata

# docker
# https://docs.docker.com/engine/install/ubuntu/
RUN apt-get update &&  apt-get install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
RUN curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
RUN add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
RUN apt-get update &&  apt-get install -y \
    docker-ce \
    docker-ce-cli \
    containerd.io

# bazel
# https://docs.bazel.build/versions/master/install-ubuntu.html
RUN apt-get update && apt-get install curl gnupg -y
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN apt-get update -y &&  apt-get install bazel -y

# bazel docker_rule dependencies
RUN apt-get update &&  apt-get install -y \
    python3 \
    python3-distutils \
    git
RUN ln /usr/bin/python3 /usr/bin/python

# other tools
RUN apt-get update && apt-get install -y \
    wget

CMD ["/bin/bash"]
