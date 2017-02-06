#/bin/bash

if [ ! -d "bin" ]; then  mkdir bin; fi

if [ ! -d "lib" ]; then  mkdir lib; fi

# ±‡“ÎlibCDC_MySQL∫ÕTests
cd prj_linux
make 
cd ..
cp -f lib/libCDC_MySQL.so bin

cp -f lib/libCDC_MySQL.so CDC_MySQL_dev

cd CDC_MySQL_dev
make
cd ..

chmod +x bin/*
