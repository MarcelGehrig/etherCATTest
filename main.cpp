#include <iostream>
#include <signal.h>

#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/core/Executor.hpp>

#include <EtherCATMain.hpp>

#include "etherCatInterface/EtherCATInterfaceElmo.hpp"
using namespace etherCATInterface;




ecmasterlib::EtherCATMain* etherCATStackPtr;

bool running = true;


static constexpr int byteSizePerSlave = 10;
static constexpr int numberOfDrivesTotal = 2;



static void signalHandler( int nSignal )
{
    std::cout << "signalHandler" << std::endl;
    etherCATStackPtr->stop();
    running = false;
    std::cout << "signalHandler" << std::endl;
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
	auto etherCATStack = ecmasterlib::EtherCATMain::createInstance(argc, argv, byteSizePerSlave*numberOfDrivesTotal);
	signal(SIGINT, signalHandler);
	//   etherCATStackPtr = etherCATStack;
	sleep(9);
	EtherCATInterfaceElmo elmoDrives = EtherCATInterfaceElmo( etherCATStack );
	//   movingchair::MovingChairEtherCAT chairEtherCAT(etherCATStack);
	//	while(1) { sleep(1); }
	//   chairEtherCAT.initElmoDrives();
	//   signal(SIGINT, signalHandler);  



	while ( !elmoDrives.initAllDrives() and running ) { 
		sleep(1); 
		log.info() << "MAIN: initAllDrives";
	};

// 	while ( running ) {
// 		log.info() << "loop";
// 		sleep(1);
// 	};
	
	auto cv = etherCATStack->getConditionalVariable();
	auto m = etherCATStack->getMutex();
	
// 	elmoDrives.initAllDrives();
	
	int count;
		elmoDrives.setDigitalOutput(0, 0xFFFFFFFF);
		elmoDrives.setDigitalOutput(1, 0xFFFFFFFF);
// 		elmoDrives.setDigitalOutput(0, 0);
// 		elmoDrives.setDigitalOutput(1, 0);
	
	while (running) {
		std::unique_lock<std::mutex> lk(*m);
		cv->wait(lk);
		lk.unlock();
		
// 		if (elmoDrives.getDigitalInputs(1) == 0) {
// 			elmoDrives.setDigitalOutput(0, 0x1);
// 		}
// 		else {
// 			elmoDrives.setDigitalOutput(0, 0x0);
// 		}
		
		
		if (count%500 == 0) {
			elmoDrives.setDigitalOutput(0, 0xFFFFFFFF);
			elmoDrives.setDigitalOutput(1, 0xFFFFFFFF);
			if (count%1000 == 0) {
				elmoDrives.setDigitalOutput(0, 0);
				elmoDrives.setDigitalOutput(1, 0);
			}
// 			log.info() << "getStatusWord 0: 0x" << std::hex << elmoDrives.getStatusWord(0);
// 			log.info() << "getStatusWord 1: 0x" << std::hex << elmoDrives.getStatusWord(1);
// 			log.info() << "getDriveStatus 0:" << elmoDrives.getDriveStatusElmo(0);
// 			log.info() << "getDriveStatus 1:" << elmoDrives.getDriveStatusElmo(1);
			log.info() << "loop DI0: " << std::hex << elmoDrives.getDigitalInputs(0);
			log.info() << "loop DI1: " << std::hex << elmoDrives.getDigitalInputs(1);
			log.info();
			log.info();
// 			log.info() << "loop: " << std::hex << elmoDrives.get(0);
// 			log.info() << "loop: " << std::hex << elmoDrives.getStatusWord(1);
// 		elmoDrives.setDigitalOutput(0, 0x0);
// 		elmoDrives.setDigitalOutput(1, 0x0);
		}
		
		
		
		count++;
	}

	//   sleep(5);
	//   





	return 0;
}