#!/bin/bash


# coredump
ulimit -c unlimited
rm -f ./core


sudo ./build/ethercattest -f 06_ENI_2Elmo_1000ms.xml -i8254x 1 1 -perf -v 4
#sudo ./build/ethercattest -f 01_ENI_1Elmo_1000ms.xml -i8254x 1 1 -perf -v 4
