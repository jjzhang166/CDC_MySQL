#/bin/bash


# ����libCDC_MySQL��Tests
cd prj_linux
make 
cd ..
cp -f lib/libCDC_MySQL.so bin

chmod +x bin/*
