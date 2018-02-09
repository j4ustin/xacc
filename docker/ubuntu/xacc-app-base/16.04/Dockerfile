from ubuntu:16.04

run mkdir /projects
workdir /projects
# Fetch library deps
run apt-get -y upgrade && apt-get -y update && apt-get -y install software-properties-common wget && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get -y install build-essential openssl libssl-dev git software-properties-common libboost-all-dev libblas-dev liblapack-dev \
    && add-apt-repository ppa:george-edison55/cmake-3.x && apt-get -y update && apt-get -y install cmake \
    && apt-get -y upgrade
# Fetch and build XACC
run git clone https://github.com/ornl-qci/xacc && cd xacc \
    && mkdir build && cd build && mkdir install \
    && cmake .. && make install
cmd ["/bin/sh"]
