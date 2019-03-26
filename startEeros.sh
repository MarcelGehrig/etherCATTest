#!/bin/bash


# coredump
ulimit -c unlimited
rm -f ./core


sudo ./buildx86/eeros_ethercattest -c HalTest1Config.json -f 10_ENI_2Elmo_1000ms_extended.xml -i8254x 1 1 -v 0 -dcmmode mastershift -b 1000
#sudo ./buildx86/eeros_ethercattest -f 10_ENI_2Elmo_1000ms_extended.xml -i8254x 1 1 -perf -v 0 -dcmmode mastershift -b 1000 
#sudo ./buildx86/eeros_ethercattest -f 09_ENI_2Elmo_250ms.xml -i8254x 1 1 -perf -v 3 -dcmmode mastershift -b 1000
#sudo ./buildx86/eeros_ethercattest -f 06_ENI_2Elmo_1000ms.xml -i8254x 1 1 -perf -v 0
#sudo ./buildx86/eeros_ethercattest -f 01_ENI_1Elmo_1000ms.xml -i8254x 1 1 -perf -v 0 -dcmmode mastershift -b 1000

#sudo ./buildx86/eeros_ethercattest -f 07_ENI_2Elmo_100ms.xml -i8254x 1 1 -perf -v 4

