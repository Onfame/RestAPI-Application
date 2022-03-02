FROM ubuntu:latest

ENV TZ=Europe/Minsk

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

WORKDIR /opt
RUN apt-get update -y \
    && apt-get upgrade -y\
    && apt-get install -y curl \
    && apt-get install -y git \
    && apt-get install -y openssl \
    && apt-get install -y cmake \
    && apt-get install -y make\
    && apt-get install -y libmongoc-1.0-0 \
    && apt-get install -y libbson-1.0-0 \
    && apt-get install -y libssl-dev \
    && apt-get install -y libsasl2-dev \
    && apt-get install -y gcc\
    && apt-get install -y g++\
    && apt-get install -y libjsoncpp-dev\
    && apt-get install -y uuid-dev\
    && apt-get install -y zlib1g-dev


WORKDIR /opt
RUN curl -L -O https://github.com/mongodb/mongo-c-driver/releases/download/1.17.0/mongo-c-driver-1.17.0.tar.gz \
  && tar xzf mongo-c-driver-1.17.0.tar.gz \
  && cd mongo-c-driver-1.17.0 \
  && mkdir cmake-build && cd cmake-build \
  && cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTS=OFF .. \
  && make -j8 \
  && make install

  WORKDIR /opt
  RUN curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.0/mongo-cxx-driver-r3.6.0.tar.gz \
    && tar -xzf mongo-cxx-driver-r3.6.0.tar.gz \
    && cd mongo-cxx-driver-r3.6.0/build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build . --target EP_mnmlstc_core \
    && cmake --build . \
    && cmake --build . --target install

  WORKDIR /opt
  RUN cd $WORK_PATH\
      && git clone https://github.com/an-tao/drogon\
      && cd drogon\
      && git submodule update --init\
      && mkdir build\
      && cd build\
      && cmake -DCMAKE_BUILD_TYPE=Release .. \
      && make install


WORKDIR /usr/src/book-service

COPY . ./

RUN cmake -DCMAKE_BUILD_TYPE=Release -B build \
    && cmake --build build