#!/bin/bash

CUR_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
TOP_DIR=$(cd $CUR_DIR/..; pwd)

cd $TOP_DIR
docker run -d \
    --restart always \
    -v ${TOP_DIR}/home/:/home/vsftpd \
    -v ${TOP_DIR}/scripts/entrypoint.sh:/entrypoint \
    -v ${TOP_DIR}/scripts/run-vsftpd.sh:/app/bin/run-vsftpd.sh \
    -p 20:20 -p 21:21 -p 21100-21110:21100-21110 \
    -e LOG_STDOUT=1 \
    -e FTP_USER=holyhand -e FTP_PASS=holyhand \
    -e PASV_ADDRESS=127.0.0.1 -e PASV_MIN_PORT=21100 -e PASV_MAX_PORT=21110 \
    -e COS_SECRETID=AKIDV7XjgOr42nMhneGdmiPs66rNioeFafeT \
    -e COS_SECRETKEY=d190cxQk0CHCtLXjhQt65tUr2yf7KI1V \
    -e COS_ENDPOINT=cos.ap-beijing.myqcloud.com \
    -e COS_APPID=1301930378 \
    -e COS_BUCKET_NAME=cosftpd-1301930378 \
    --entrypoint=/entrypoint \
    --name vsftpd \
    hzcsk8s.io/cosftpd
cd - >/dev/null
