FROM ubuntu:22.04
ENV TZ="Asia/Singapore"
ENV DEBIAN_FRONTEND=noninteractive
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt update && apt install -y git libuhd-dev uhd-host build-essential cmake ninja-build \
    libfftw3-dev libmbedtls-dev libboost-program-options-dev libconfig++-dev libsctp-dev
RUN uhd_images_downloader
RUN git clone https://github.com/roskeys/imsi-catcher /root/srsran
WORKDIR /root/srsran
RUN cmake -B build -G Ninja && ninja -C build
