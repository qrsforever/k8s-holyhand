#!/bin/bash

# Create home dir and update vsftpd user db:
mkdir -p "/home/vsftpd/${FTP_USER}" "/var/ftp" "/app/config"
chown -R ftp:ftp /home/vsftpd/ /var/ftp

echo -e "${FTP_USER}\n${FTP_PASS}" > /app/config/virtual_users.txt
/usr/bin/db_load -T -t hash -f /app/config/virtual_users.txt /app/config/virtual_users.db

cat > /app/config/vsftpd.conf << EOB
background=NO
anonymous_enable=NO
local_enable=YES
guest_enable=YES
write_enable=YES
virtual_use_local_privs=YES
pam_service_name=vsftpd_virtual
user_sub_token=\$USER
local_root=/home/vsftpd/\$USER
chroot_local_user=YES
allow_writeable_chroot=YES
hide_ids=YES
xferlog_enable=YES
xferlog_file=/var/log/vsftpd/vsftpd.log
port_enable=YES
connect_from_port_20=YES
ftp_data_port=20
seccomp_sandbox=NO
EOB

echo "pasv_address=${PASV_ADDRESS}" >> /app/config/vsftpd.conf
echo "pasv_max_port=${PASV_MAX_PORT}" >> /app/config/vsftpd.conf
echo "pasv_min_port=${PASV_MIN_PORT}" >> /app/config/vsftpd.conf
echo "pasv_addr_resolve=${PASV_ADDR_RESOLVE}" >> /app/config/vsftpd.conf
echo "pasv_enable=${PASV_ENABLE}" >> /app/config/vsftpd.conf
echo "file_open_mode=${FILE_OPEN_MODE}" >> /app/config/vsftpd.conf
echo "local_umask=${LOCAL_UMASK}" >> /app/config/vsftpd.conf
echo "xferlog_std_format=${XFERLOG_STD_FORMAT}" >> /app/config/vsftpd.conf
echo "pasv_promiscuous=${PASV_PROMISCUOUS}" >> /app/config/vsftpd.conf
echo "port_promiscuous=${PORT_PROMISCUOUS}" >> /app/config/vsftpd.conf

# Get log file path
export LOG_FILE=`grep xferlog_file /app/config/vsftpd.conf|cut -d= -f2`

# stdout server info:
if [ ! $LOG_STDOUT ]; then
cat << EOB
	SERVER SETTINGS
	---------------
	· FTP User: $FTP_USER
	· FTP Password: $FTP_PASS
	· Log file: $LOG_FILE
	· Redirect vsftpd log to STDOUT: No.
EOB
else
    /usr/bin/ln -sf /dev/stdout $LOG_FILE
fi

# Run vsftpd:
# &>/dev/null /app/bin/vsftpd /app/config/vsftpd.conf
