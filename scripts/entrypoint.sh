#!/bin/bash

CUR_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
BIN_DIR=/app/bin

while (( 1 ))
do
    echo "setup vsftpd environments"
    ${BIN_DIR}/run-vsftpd.sh

    echo "start main progress"
    ${BIN_DIR}/cosftpd

    sleep 3
done
