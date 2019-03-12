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


static constexpr int byteSizePerSlave = 32;
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

	
	
	auto cv = etherCATStack->getConditionalVariable();
	auto m = etherCATStack->getMutex();
	auto cv2 = etherCATStack->getConditionalVariable2();
	auto m2 = etherCATStack->getMutex2();


	while ( !elmoDrives.initAllDrives() and running ) { 
		sleep(1); 
		log.info() << "MAIN: initAllDrives";
// 		cv->notify_one();
	};

// 	while ( running ) {
// 		log.info() << "loop";
// 		sleep(1);
// 	};
	
// 	elmoDrives.initAllDrives();
	
	int count = 0;
		elmoDrives.setDigitalOutput(0, 0xFFFFFFFF);
		elmoDrives.setDigitalOutput(1, 0xFFFFFFFF);
// 		elmoDrives.setDigitalOutput(0, 0);
// 		elmoDrives.setDigitalOutput(1, 0);
	
	int delay = 0;
	
// 	etherCATStack->wait = true;
	while (running) {
		std::unique_lock<std::mutex> lk(*m);
		cv->wait(lk);
		lk.unlock();
		
// 		if ( count == 10000 ) {
// 			log.info() << "AAAAAAA: start waiting";
// 			etherCATStack->wait = true;
// 			cv2->notify_one();
// 		}
		
// 		if ( count%200 == 0 ) {
// 					log.info() << "                               position: " << elmoDrives.getPositionActualValue(0);
// 		}
// 		
// 		if ( count%500 == 0 ) {
// 			log.info() << "delay: " << delay;
// 			delay = 0;
// 			if ( elmoDrives.getDigitalInputs(1) == 0x3f0000 ) {
// 				elmoDrives.setDigitalOutput(0, 0xFFFFFFFF);
// 			}
// 			else {
// 				elmoDrives.setDigitalOutput(0, 0x0);
// 			}
// 			log.info() << "loop DI1: " << std::hex << elmoDrives.getDigitalInputs(1);
// 		}
// 		if ( elmoDrives.getDigitalInputs(1) == 0x3f0000 ) {
// 			delay++;
// 		}
		
		
		if (count == 3000 ) {
			std::cout << "enableTouchProbe1" << std::endl;
// 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableTouchProbe1);
			elmoDrives.setTouchProbeFunction(0, 0x31);
		}
		if (count == 4000) {
// 			std::cout << "enabling sampling" << std::endl;
// 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe1);
		}
		
// 		elmoDrives.getTouchProbeIsEnabled(1);
		if (!elmoDrives.getTouchProbeIsEnabled(0)) {
// 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableTouchProbe1);
// 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe1);
// 			std::cout << "enabeling enableSamplingAtThePositiveEdgeOfTouchProbe1" << std::endl;
		}
		else {
// 			std::cout << "index enabled" << std::endl;
			if ( elmoDrives.getIndexPulseIsCaptured(0) and count%500 == 0) {
				std::cout << "index positive captured at: " << elmoDrives.getCapturedPositionPositivePulse(0);
			}
		}
		
		
		if ( count%1000 == 0 ) {
			std::cout << "MAIN: getTouchProbeStatus(0): 0x" << std::hex << elmoDrives.getTouchProbeStatus(0) << std::endl;
			std::cout << "MAIN: getTouchProbePosition(0): 0x" << std::hex << elmoDrives.getTouchProbePos1Positive(0) << std::endl;
			log.info() << "                               position: " << elmoDrives.getPositionActualValue(0);
		}
		
		
		count++;
		cv2->notify_one();
	}

	//   sleep(5);
	//   





	return 0;
}