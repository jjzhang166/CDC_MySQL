#/bin/bash

if [ ! -d "bin" ]; then  mkdir bin; fi

if [ ! -d "lib" ]; then  mkdir lib; fi

# 解压并安装cJSON库
tar xzvf cJSON-1.2.0.tar.gz
cd cJSON-1.2.0
./configure
make 
sudo make install
cd ..

# 解压并编译CppUnit
tar xvf CppUnit.tar
cd CppUnit/prj_linux
make 
cd ../../

cp -f CppUnit/lib/libCppUnit.so lib/libCppUnit.so
cp -f CppUnit/lib/libCppUnit.so bin/libCppUnit.so

# 编译libCDC_MySQL和Tests
cd prj_linux
make 
cd ..
cp -f lib/libCDC_MySQL.so bin/libCDC_MySQL.so

cp -f lib/libCDC_MySQL.so CDC_MySQL_dev/libCDC_MySQL.so

cd CDC_MySQL_dev
make
cd ..

chmod +x bin/*
