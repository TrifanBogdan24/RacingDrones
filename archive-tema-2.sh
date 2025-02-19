#!/bin/bash


rm -rf /mnt/c/Users/trifa/Downloads/EGC-Tema-2.zip


# Zip everything in the specified directories, excluding the build/.vs folder
zip -r /mnt/c/Users/trifa/Downloads/EGC-Tema-2.zip \
    build \
    deps \
    assets \
    tools \
    infra \
    docs \
    src/components \
    src/utils \
    src/core \
    src/main.cpp \
    src/lab_m1/lab_list.h \
    src/lab_m1/DroneGame \
    archive-tema-2.sh \
    -x "build/.vs/*" \
    -x "src/.git"
