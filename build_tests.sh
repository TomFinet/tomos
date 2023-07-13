DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

rm "$DIR/build/CMakeFiles" "$DIR/build/kernel" "$DIR/build/root" \
   "$DIR/build/cmake_install.cmake" "$DIR/build/CMakeCache.txt"  \
   "$DIR/build/CopyOfCMakeCache.txt" "$DIR/build/install_manifest.txt" \
   "$DIR/build/MakeFile" "$DIR/build/tomos.img" -rf

mkdir -p "$DIR/build" && cd "$DIR/build"
cmake .. -DCMAKE_TOOLCHAIN_FILE=toolchain/CMakeToolchain.txt -DKTESTS=True
make preprocess_linker_script
make install

# make returns 2 if errors
if make --directory="."
then
    make image 
    make qemu
else
    exit 1
fi