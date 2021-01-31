FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt install -y \
      cmake \
      gcc g++ \
      gdb \
      clang \
      clang-format \
      ninja-build \
      vim \
      git \
      libopencv-dev \
      apt-utils build-essential gdbserver openssh-server rsync

# Taken from - https://docs.docker.com/engine/examples/running_ssh_service/#environment-variables
RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# 22 for ssh server. 7777 for gdb server.
EXPOSE 22 7777

# Create dev user with password 'dev'
RUN useradd -ms /bin/bash dev
RUN echo 'dev:dev' | chpasswd

# Upon start, run ssh daemon
CMD ["/usr/sbin/sshd", "-D"]
