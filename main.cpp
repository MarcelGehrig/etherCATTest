#include <iostream>
#include <signal.h>

#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/core/Executor.hpp>

#include <EtherCATMain.hpp>


ethercat::EtherCATMain* etherCATStackPtr;

bool running = true;


static constexpr int byteSizePerSlave = 34;
static constexpr int numberOfDrivesTotal = 2;



static void signalHandler( int nSignal )
{
    std::cout << "signalHandler" << std::endl;
    etherCATStackPtr->stop();
    running = false;
}
// 

int main(int argc, char **argv) {
  signal(SIGINT, signalHandler);
  
  using namespace eeros::logger;
  

  StreamLogWriter w(std::cout);
  Logger log;
  log.set(w);

  log.info() << "Hello, EEROS";
  
  
  
  
  // EtherCAT
  // ////////////////////////////////////////////////////////////////////////
  auto etherCATStack = ethercat::EtherCATMain::createInstance(argc, argv, byteSizePerSlave*numberOfDrivesTotal);
  signal(SIGINT, signalHandler);
//   etherCATStackPtr = etherCATStack;
  sleep(9);
//   movingchair::MovingChairEtherCAT chairEtherCAT(etherCATStack);
  //	while(1) { sleep(1); }
//   chairEtherCAT.initElmoDrives();
//   signal(SIGINT, signalHandler);  
  

  
  while ( running ) {
    log.info() << "loop";
    sleep(1);
  };
  
//   sleep(5);
//   
  
  
  
  
 
  return 0;
}