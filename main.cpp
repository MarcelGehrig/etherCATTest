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



	while ( elmoDrives.initAllDrives() ) { sleep(0.001); };

// 	while ( running ) {
// 		log.info() << "loop";
// 		sleep(1);
// 	};
	
	auto cv = etherCATStack->getConditionalVariable();
	auto m = etherCATStack->getMutex();
	
	int count;
// 		elmoDrives.setDigitalOutput(0, 0xFFFFFFFF);
// 		elmoDrives.setDigitalOutput(1, 0xFFFFFFFF);
		elmoDrives.setDigitalOutput(0, 0);
		elmoDrives.setDigitalOutput(1, 0);
	
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
		
		if (count%1000 == 0) {
			log.info() << "loop: " << std::hex << elmoDrives.getDigitalInputs(0);
			log.info() << "loop: " << std::hex << elmoDrives.getDigitalInputs(1);
// 		elmoDrives.setDigitalOutput(0, 0x0);
// 		elmoDrives.setDigitalOutput(1, 0x0);
		}
		
		count++;
	}

	//   sleep(5);
	//   





	return 0;
}