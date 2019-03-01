cmake_minimum_required(VERSION 2.8)
 
project(ethercattest)

## EEROS
## ////////////////////////////////////////////////////////////////////////////
find_package(EEROS REQUIRED)
include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
 
 
 
add_executable(ethercattest 
								main.cpp
								etherCatInterface/EtherCATInterfaceBase.cpp
								etherCatInterface/EtherCATInterfaceElmo.cpp
)

target_link_libraries(ethercattest eeros ucl ecmasterlib ${CMAKE_DL_LIBS})
add_subdirectory(etherCatInterface)