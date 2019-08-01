#!/bin/sh
mkdir build
cd build
cmake ../
make
mkdir temp
cp RepoHub.vpk temp/
cd temp/
unzip RepoHub.vpk
cp eboot.bin ../eboot.bin
cd ../
rm -r temp/

for i in $@
do
    echo destroy | nc $i 1338
    curl --ftp-method nocwd -T eboot.bin ftp://$i:1337/ux0:/app/rbdv00002/
    echo launch rbdv00002 | nc $i 1338
done
