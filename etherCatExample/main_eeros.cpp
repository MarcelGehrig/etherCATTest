#include <iostream>
#include <signal.h>

#include <eeros/core/Executor.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/hal/HAL.hpp>
#include "MainSequence.hpp"
#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include "globalConfig.hpp"

#include <EtherCATMain.hpp>

#include "../etherCatInterface/EtherCATInterfaceElmo.hpp"

using namespace etherCATInterface;
using namespace eeros::control;
using namespace eeros::safety;
using namespace eeros::logger;
using namespace eeros::sequencer;
using namespace eeros::hal;


bool running = true;
ecmasterlib::EtherCATMain* etherCATStackPtr;
static constexpr int byteSizePerSlave = 32;
static constexpr int numberOfDrivesTotal = 2;
static constexpr double dt = 0.001;

static void signalHandler( int nSignal )
{
	std::cout << "signalHandler" << std::endl;
	SafetySystem::exitHandler();
	Sequencer::instance().abort();
	etherCATStackPtr->stop();
	running = false;
	std::cout << "signalHandler" << std::endl;
}
// 

int main(int argc, char **argv) {
	signal(SIGINT, signalHandler);

	// Logger
	// ////////////////////////////////////////////////////////////////////////
	StreamLogWriter w(std::cout);
	Logger log;
	log.set(w);
	global::log = &log;

	log.info() << "Hello, EEROS";


	// EtherCAT
	// ////////////////////////////////////////////////////////////////////////
// 	struct config {
// 		std::string::string eni = "asdf";
// 		
// 	};
	ecmasterlib::EtherCATMain* etherCATStack = ecmasterlib::EtherCATMain::createInstance(argc, argv, byteSizePerSlave*numberOfDrivesTotal);
	global::etherCATStack = etherCATStack;
	signal(SIGINT, signalHandler);
	sleep(9);
	EtherCATInterfaceElmo elmoDrives = EtherCATInterfaceElmo( etherCATStack );
	global::elmoDrives = &elmoDrives;
	bool allDrivesAreSwitchedOn = false;
// 	while (!allDrivesAreSwitchedOn) {
// 		allDrivesAreSwitchedOn = elmoDrives.switchOnAllDrives();
// 		usleep(1e6);	
// 	}
	//   signal(SIGINT, signalHandler);  
	
	// HAL
	// ////////////////////////////////////////////////////////////////////////
// 	HAL& hal = HAL::instance();
// 	hal.readConfigFromFile(&argc, argv);
	
	// Control system
	// ////////////////////////////////////////////////////////////////////////
	MyControlSystem CS(dt, elmoDrives, numberOfDrivesTotal, log);
	global::CS = &CS;
	
	// Safety system
	// ////////////////////////////////////////////////////////////////////////
	MySafetyProperties properties(CS, elmoDrives, dt);
	global::safetyProperties = &properties;
	SafetySystem SS(properties, dt);
	global::SS = &SS;
//	CS.timedomain.registerSafetyEvent(SS, properties.doEmergency);
	
// 	// Sequencer
	// ////////////////////////////////////////////////////////////////////////
	auto& sequencer = Sequencer::instance();
	global::sequencer = &sequencer;
	MainSequence mainSequence("MainSequence", sequencer, SS, properties, CS, elmoDrives);
// 	MainSequence mainSequence("MainSequence");
// 	global::mainSequence = &mainSequence;
// 	MainSequence mainSequence("Main Sequence", sequencer);
	sequencer.addSequence(mainSequence);
	mainSequence.start();
	
	
	// Executor
	// ////////////////////////////////////////////////////////////////////////
	auto &executor = eeros::Executor::instance();
	executor.syncWithEtherCATSTack(etherCATStack);
	executor.setMainTask(SS);
// 	SS.triggerEvent(properties.initDrives);
	
	executor.run();
	
	mainSequence.waitAndTerminate();
	
	log.info()<< "main_eeros finished";
	
	
	return 0;
}
