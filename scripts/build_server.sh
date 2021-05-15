#!/bin/bash

set -e

function build_server() {
    cd "$PROJECT_DIR"

    if [[ $(echo "$1" | tr "[:lower:]" "[:upper:]") == "DEBUG" ]]; then
        BUILD_TYPE="Debug"
    else
        BUILD_TYPE="Release"
    fi

    cmake -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    cmake --build build --config "$BUILD_TYPE"
    cmake -P build/cmake_install.cmake
}

cd "$(dirname "$0")" && source env_prepare
build_server "$1"
