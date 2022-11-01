#!/bin/bash

CUR_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
BIN_DIR=/app/bin

## setup vsftpd environments
${BIN_DIR}/run-vsftpd.sh

## start main progress
sleep 100000
# ${BIN_DIR}/cosftpd
