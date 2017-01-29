#/bin/bash


# ±‡“ÎlibCDC_MySQL∫ÕTests
cd prj_linux
make 
cd ..
cp -f lib/libCDC_MySQL.so bin

cp -f lib/libCDC_MySQL.so CDC_MySQL_dev

chmod +x bin/*
