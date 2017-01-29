#/bin/bash

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
cp -f CppUnit/lib/* lib
cp -f CppUnit/lib/* bin

# 编译libCDC_MySQL和Tests
cd prj_linux
make 
cd ..
cp -f lib/libCDC_MySQL.so bin

cp -f lib/libCDC_MySQL.so CDC_MySQL_dev

chmod +x bin/*
