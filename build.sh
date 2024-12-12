#!/bin/bash

# Configuration
CXX=clang++
PROJECT_NAME=sql-bundler
CXXFLAGS="-std=c++17 -Wall -Wextra -Werror"
DEBUG_FLAGS="-g -O0"
RELEASE_FLAGS="-O3"

# Create build directory
mkdir -p build

# Build function
build() {
    BUILD_TYPE=$1
    FLAGS=$2

    echo "Building ${BUILD_TYPE} version..."

    $CXX $CXXFLAGS $FLAGS main.cpp -o build/$PROJECT_NAME-$BUILD_TYPE

    if [ $? -eq 0 ]; then
        echo "✅ ${BUILD_TYPE} build successful: build/$PROJECT_NAME-$BUILD_TYPE"
    else
        echo "❌ ${BUILD_TYPE} build failed"
        exit 1
    fi
}

# Clean build directory
clean() {
    echo "Cleaning build directory..."
    rm -rf build/*
}

# Parse arguments
case "$1" in
    "clean")
        clean
        ;;
    "debug")
        build "debug" "$DEBUG_FLAGS"
        ;;
    "release")
        build "release" "$RELEASE_FLAGS"
        ;;
    *)
        echo "Usage: $0 {debug|release|clean}"
        echo "  debug   - Build with debug symbols"
        echo "  release - Build with optimizations"
        echo "  clean   - Clean build directory"
        exit 1
        ;;
esac