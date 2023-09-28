DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

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