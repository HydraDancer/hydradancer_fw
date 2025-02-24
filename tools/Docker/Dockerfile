FROM ubuntu:22.04
ARG UID #needed to avoid conflicts when editing files in Docker vs the host

RUN apt-get update --yes && apt-get install git wget tar make python3 python3-pip cmake --yes
RUN wget https://github.com/hydrausb3/riscv-none-elf-gcc-xpack/releases/download/12.2.0-1/xpack-riscv-none-elf-gcc-12.2.0-1-linux-x64.tar.gz
RUN tar xf xpack-riscv-none-elf-gcc-12.2.0-1-linux-x64.tar.gz -C /opt xpack-riscv-none-elf-gcc-12.2.0-1/
RUN python3 -m pip install clang-format autopep8 pylint


RUN useradd -u $UID -m user
USER user
WORKDIR /home/user
RUN echo "PATH=$PATH:/opt/xpack-riscv-none-elf-gcc-12.2.0-1/bin" > ~/.bashrc
WORKDIR /home/user/mounted

