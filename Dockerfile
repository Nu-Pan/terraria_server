# alpine だと terraria 動かなかったので
FROM debian:latest

# Config
ENV PACKAGE_URL=https://terraria.org/system/dedicated_servers/archives/000/000/037/original/terraria-server-1403.zip
ENV INSTALL_PREFIX=/TerrariaServer
ENV BUILD_PREFIX=/build
ENV COMMANDS_PREFIX=/commands

# スクリプト類を展開
ADD setup_scripts ${BUILD_PREFIX}
ADD runtime_scripts ${COMMANDS_PREFIX}

# apt update が激重なのでレイヤーを分けてキャッシュを効かせる
RUN cp "${BUILD_PREFIX}/source.list" "/etc/apt/sources.list" \
&&  apt-get update

# インストール
RUN mkdir -p ${BUILD_PREFIX} \
&&  cd ${BUILD_PREFIX} \
&&  ${BUILD_PREFIX}/prepare_package.sh ${PACKAGE_URL} ${INSTALL_PREFIX} \
&&  ${BUILD_PREFIX}/prepare_environment.sh ${INSTALL_PREFIX} ${COMMANDS_PREFIX} \
&&  cd / \
&&  rm -rf ${BUILD_PREFIX} \
&&  apt-get autoremove -y \
&&  rm -rf /var/cache/apt \
&&  apt-get clean

CMD ["/commands/run_server.sh", "/TerrariaServer", "/commands"]
