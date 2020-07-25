# alpine だと terraria 動かなかったので
FROM ubuntu:latest

# Config
ENV PACKAGE_URL=https://terraria.org/system/dedicated_servers/archives/000/000/037/original/terraria-server-1403.zip
ENV INSTALL_PREFIX=/TerrariaServer
ENV BUILD_PREFIX=/build
ENV COMMANDS_PREFIX=/commands

# スクリプト類を展開
ADD setup_scripts ${BUILD_PREFIX}
ADD runtime_scripts ${COMMANDS_PREFIX}

# apt-get update が激重なのでレイヤーを分けてキャッシュを効かせる
RUN sed -i.bak -e "s%http://[^ ]\+%http://ftp.jaist.ac.jp/pub/Linux/ubuntu/%g" /etc/apt/sources.list
RUN apt-get update

# インストール
RUN mkdir -p ${BUILD_PREFIX}
RUN cd ${BUILD_PREFIX}
RUN ${BUILD_PREFIX}/prepare_package.sh ${PACKAGE_URL} ${INSTALL_PREFIX}
RUN ${BUILD_PREFIX}/prepare_environment.sh ${INSTALL_PREFIX} ${COMMANDS_PREFIX}
RUN cd
RUN rm -rf ${BUILD_PREFIX}
RUN apt-get autoremove -y
RUN rm -rf /var/cache/apt
RUN apt-get clean

#bazel
RUN apt-get install curl gnupg -y
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN apt-get update -y &&  apt-get install bazel -y

CMD ["/commands/run_server.sh", "/TerrariaServer", "/commands"]
