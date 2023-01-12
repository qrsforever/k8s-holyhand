1. source _.env
2. docker-compose build cosftpd-devel (if need）
    docker-compose push cosftpd-devel
3. docker-compose build cosftpd
    docker-compose push cosftpd
4. docker-compose run cosftpd (source _.env)
