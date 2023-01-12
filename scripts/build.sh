#!/bin/bash

CUR_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
TOP_DIR=$(cd ${CUR_DIR}/..; pwd)

[[ "${BASH_SOURCE[0]}" != "" ]] && SOURCED=1 || SOURCED=0

cd $TOP_DIR

# docker build -t hzcsk8s.io/cosftpd-devel -f dockerfiles/Dockerfile.dev .
docker build -t hzcsk8s.io/cosftpd -f dockerfiles/Dockerfile.cosftpd .

cd >/dev/null
