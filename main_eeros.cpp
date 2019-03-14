#include <iostream>
#include <signal.h>

#include <eeros/core/Executor.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/hal/HAL.hpp>
#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include "MainSequence.hpp"

#include <EtherCATMain.hpp>

#include "etherCatInterface/EtherCATInterfaceElmo.hpp"

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

	log.info() << "Hello, EEROS";


	// EtherCAT
	// ////////////////////////////////////////////////////////////////////////
	ecmasterlib::EtherCATMain* etherCATStack = ecmasterlib::EtherCATMain::createInstance(argc, argv, byteSizePerSlave*numberOfDrivesTotal);
	signal(SIGINT, signalHandler);
	sleep(9);
	EtherCATInterfaceElmo elmoDrives = EtherCATInterfaceElmo( etherCATStack );
	bool allDrivesAreSwitchedOn = false;
	while (!allDrivesAreSwitchedOn) {
		allDrivesAreSwitchedOn = elmoDrives.switchOnAllDrives();
		usleep(1e6);	
	}
	//   signal(SIGINT, signalHandler);  
	
	// HAL
	// ////////////////////////////////////////////////////////////////////////
// 	HAL& hal = HAL::instance();
// 	hal.readConfigFromFile(&argc, argv);
	
	// Control system
	// ////////////////////////////////////////////////////////////////////////
	MyControlSystem controlSys(dt, etherCATStack, &elmoDrives);
	
	// Safety system
	// ////////////////////////////////////////////////////////////////////////
	MySafetyProperties properties(controlSys, dt);
	SafetySystem safetySys(properties, dt);
	controlSys.timedomain.registerSafetyEvent(safetySys, properties.doEmergency);
	
	// Sequencer
	// ////////////////////////////////////////////////////////////////////////
	auto& sequencer = Sequencer::instance();
	MainSequence mainSequence("Main Sequence", sequencer, safetySys, properties, controlSys, 3.14/10);
	sequencer.addSequence(mainSequence);
	mainSequence.start();
	
	
	// Executor
	// ////////////////////////////////////////////////////////////////////////
	auto &executor = eeros::Executor::instance();
	executor.syncWithEtherCATSTack(etherCATStack);
	executor.setMainTask(safetySys);
	safetySys.triggerEvent(properties.doSystemOn);
	
	executor.run();
	
	mainSequence.waitAndTerminate();
	
	
	
	return 0;
}