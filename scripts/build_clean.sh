DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SRC_ROOT="$DIR/../"

rm "$SRC_ROOT/build/CMakeFiles" "$SRC_ROOT/build/kernel" "$SRC_ROOT/build/root" \
   "$SRC_ROOT/build/cmake_install.cmake" "$SRC_ROOT/build/CMakeCache.txt"  \
   "$SRC_ROOT/build/CopyOfCMakeCache.txt" "$SRC_ROOT/build/install_manifest.txt" \
   "$SRC_ROOT/build/MakeFile" "$SRC_ROOT/build/tomos.img" -rf

mkdir -p "$SRC_ROOT/build" && cd "$SRC_ROOT/build"
cmake .. -DCMAKE_TOOLCHAIN_FILE=$SRC_ROOT/toolchain/CMakeToolchain.txt

make linker_script
make install

# make returns 2 if errors
if make --directory="."
then
    make image 
    make qemu
else
    exit 1
fi
