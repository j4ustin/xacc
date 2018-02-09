from ubuntu:16.04

run mkdir /projects
workdir /projects
# Fetch library deps
run apt-get -y update && apt-get -y install software-properties-common wget && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get -y install build-essential openssl libssl-dev git software-properties-common libboost-all-dev libblas-dev liblapack-dev \
    python3 python3-pip && add-apt-repository ppa:george-edison55/cmake-3.x && apt-get -y update && apt-get -y install cmake \
    && apt-get -y upgrade && pip3 install pyquil
# Fetch and build XACC
run git clone https://github.com/j4ustin/xacc && cd xacc \
    && mkdir build && cd build && mkdir install \
    && cmake .. -DPYTHON_EXECUTABLE=/usr/bin/python3 && make install
# Install XACC plugins
run export PATH=/usr/local/xacc/bin:$PATH \
    && cd /projects && mkdir plugin-installs && cd plugin-installs \
    && wget https://github.com/ORNL-QCI/ScaffCC/releases/download/v2.0/scaffold_2.0_amd64.deb \
    && apt-get install -y $(dpkg --info scaffold_2.0_amd64.deb | grep Depends | sed "s/.*ends:\ //" | sed 's/,//g') \
    && dpkg -i scaffold_2.0_amd64.deb && xacc-install-plugins.py -j 1 -p xacc-scaffold \
    && xacc-install-plugins.py -j 1 -p xacc-python xacc-dwave ibm rigetti
cmd ["/bin/sh"]
