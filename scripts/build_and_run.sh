#!/usr/bin/env bash
set -e
# Simple script to configure, build, test and run on Linux
BUILD_DIR=build
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j$(nproc)

# run tests if built
if [ -f tests ]; then
  ctest --output-on-failure || true
fi

# run main with arguments passed to this script
# e.g. ./build_and_run.sh --temp 45 --run-seconds 60
./eae_firmware "$@"
