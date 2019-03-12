#include <iostream>
#include <signal.h>

#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
// #include <eeros/safety/SafetySystem.hpp>
#include <eeros/core/Executor.hpp>

#include <EtherCATMain.hpp>

#include "etherCatInterface/EtherCATInterfaceElmo.hpp"
using namespace etherCATInterface;
using namespace eeros::control;
// using namespace eeros::safety;


bool running = true;
ecmasterlib::EtherCATMain* etherCATStackPtr;
static constexpr int byteSizePerSlave = 32;
static constexpr int numberOfDrivesTotal = 2;
static constexpr double dt = 0.001;


class ControlSystem;
// class SafetyPropertiesTest : public SafetyProperties {
// public:
// 	SafetyPropertiesTest(double ts);
// 	
// 	ControlSystem* controlSystem;
// 
// 	SafetyEvent seElmosInitialized;
// 	
// 	SafetyLevel slInitializingElmos;
// 	SafetyLevel slElmosInitialized;
// };

class ControlSystem {
public:
// 	ControlSystem(ecmasterlib::EtherCATMain* etherCATStack, double dt, EtherCATInterfaceElmo* elmoDrives) {
	ControlSystem(ecmasterlib::EtherCATMain* etherCATStack, double dt, EtherCATInterfaceElmo* elmoDrives) {
// 		i.getOut().getSignal().setName("integrator output");
// 		i.getIn().connect(c.getOut());
// 		checker.setName("check integrator level");
// 		checker.getIn().connect(i.getOut());
	}

// 	Constant<> c;
// 	I<> i;
// 	SignalChecker<> checker;
};

// SafetyPropertiesTest::SafetyPropertiesTest(double ts) : 
// 	seStartRampingUp("start ramping up"),
// 	seReset("reset"),
// 	slStart("start"),
// 	slRampingUp("ramping up")
// 	{	
// 	
// 	// ############ Add levels ############
// 	addLevel(slStart);
// 	addLevel(slRampingUp);
// 	
// 	// ############ Add events to the levels ############
// 	slStart.addEvent(seStartRampingUp, slRampingUp, kPrivateEvent);
// 	slRampingUp.addEvent(seReset, slStart, kPublicEvent);
// 
// 	// Define and add level functions
// 	slStart.setLevelAction([this,ts](SafetyContext* privateContext) {
// 		controlSystem->i.disable();
// 		controlSystem->i.setInitCondition(0);
// 		if(slStart.getNofActivations() * ts > 3) {
// 			privateContext->triggerEvent(seStartRampingUp);
// 			controlSystem->checker.reset();
// 			controlSystem->i.enable();
// 		}
// 	});
// 
// 	// Define entry level
// 	setEntryLevel(slStart);	
// }

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
	ecmasterlib::EtherCATMain* etherCATStack = ecmasterlib::EtherCATMain::createInstance(argc, argv, byteSizePerSlave*numberOfDrivesTotal);
	signal(SIGINT, signalHandler);
	//   etherCATStackPtr = etherCATStack;
	sleep(9);
	EtherCATInterfaceElmo elmoDrives = EtherCATInterfaceElmo( etherCATStack );
	bool allDrivesAreSwitchedOn = false;
	while (!allDrivesAreSwitchedOn) {
		allDrivesAreSwitchedOn = switchOnAllDrives();
		usleep(1e6);	
	}
	//   signal(SIGINT, signalHandler);  
	
	// Control system
	// ////////////////////////////////////////////////////////////////////////
	ControlSystem* controlSystem;
// 	controlSystem = new ControlSystem(etherCATStack, dt, elmoDrives);
	controlSystem = new ControlSystem(etherCATStack, dt, &elmoDrives);
	
	
	// Executor
	// ////////////////////////////////////////////////////////////////////////
	auto &executor = eeros::Executor::instance();
	executor.syncWithEtherCATSTack(etherCATStack);
// 	executor.setMainTask(safetySystem);
	
	
	
	return 0;
}