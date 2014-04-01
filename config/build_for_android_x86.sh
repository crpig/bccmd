#!/bin/sh

rm CMakeCache.txt
rm -rf CMakeFiles

cmake -DCMAKE_TOOLCHAIN_FILE=~/Android/Sdk/ndk-bundle/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=x86 \
      -DANDROID_PLATFORM=21 \
      ..

echo "Done for Android!!"
