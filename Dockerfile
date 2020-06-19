FROM debian:latest

ENV PACKAGE_URL="https://www.terraria.org/system/dedicated_servers/archives/000/000/039/original/terraria-server-1405.zip"
ENV INSTALL_PREFIX="/TerrariaServer"
ENV BUILD_PREFIX="/build"
ENV SCRIPT_PREFIX="/script"

ADD setup_scripts $SCRIPT_PREFIX

RUN apt-get update && apt-get upgrade \
    && mkdir -p $BUILD_PREFIX \
    && cd $BUILD_PREFIX \
    && $SCRIPT_PREFIX/prepare_package.sh $PACKAGE_URL $INSTALL_PREFIX \
    && $SCRIPT_PREFIX/prepare_environment.sh $INSTALL_PREFIX \
    && cd / \
    && rm -rf $SCRIPT_PREFIX $BUILD_PREFIX \
    && apt-get autoremove -y \
    && apt-get clean
