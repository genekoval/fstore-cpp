FROM git.aurora.aur/aurora/alpine AS builder

RUN apk update \
 && apk upgrade

RUN apk add --no-cache \
    clang \
    cmake \
    crypto++-dev \
    curl-dev \
    file-dev \
    git \
    libuuid \
    pkgconfig \
    postgresql16 \
    samurai \
    util-linux-dev \
    yaml-cpp-dev

COPY . .

RUN cmake --workflow --preset=docker
