```
LC_ALL=C diff -ru ../vsftpd-3.0.2 vsftpd-3.0.2 | grep -v '^Only in' > vsftpd-3.0.2.patch
cd /opt/build/vsftpd-3.0.2
patch -p2 < ../patch/vsftpd-3.0.2.patch
```

```
LC_ALL=C diff -u ../cos-c-sdk-v5-5.0.16 cos-c-sdk-v5-5.0.16 | grep -v '^Only in' > cos-c-sdk-v5-5.0.16.patch
cd /opt/build/cos-c-sdk-v5-5.0.16
patch -p2 < ../patch/cos-c-sdk-v5-5.0.16.patch
```
