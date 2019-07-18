#!/bin/sh
make
mkdir temp
cp repo-browser.vpk temp/
cd temp/
unzip repo-browser.vpk
cp eboot.bin ../eboot.bin
cd ../
rm -r temp/
echo destroy | nc 192.168.1.7 1338
curl --ftp-method nocwd -T eboot.bin ftp://192.168.1.7:1337/ux0:/app/rbdv00002/
echo launch rbdv00002 | nc 192.168.1.7 1338

