 
mkdir -p build;
cd build;

cmake -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=../install/ ..
make
make install

cd ..