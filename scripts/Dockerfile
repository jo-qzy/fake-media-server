FROM debian:10.9

RUN sed -i 's/\(security\|deb\).debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list
RUN apt-get update &&               \
    apt-get -y install              \
    cmake make g++ openssh-server

RUN (   echo 'PORT 2222';                                   \
        echo 'ListenAddress 0.0.0.0';                       \
        echo 'PermitRootLogin yes';                         \
        echo 'PasswordAuthentication yes';                  \
        echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
    ) > /etc/ssh/sshd_config &&                             \
    mkdir /run/sshd &&                                      \
    yes password | passwd root

CMD ["/usr/sbin/sshd", "-d", "-e", "-f", "/etc/ssh/sshd_config"]
