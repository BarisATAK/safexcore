pacman -S autoconf automake libtool unzip
cd protobuf
git submodule update --init --recursive
./autogen.sh
./configure
make
make install
cd ../../src/rpc/proto
protoc.exe --cpp_out=../ transactions.proto
echo "Protobuf dep installed"
