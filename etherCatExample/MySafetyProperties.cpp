#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"

#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/OutputAction.hpp>
#include <eeros/core/Executor.hpp>

#include <unistd.h>
// #include <iostream>
// #include <vector>
#include <initializer_list>

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;


MySafetyProperties::MySafetyProperties(MyControlSystem& controlSys, double dt) : 
	controlSys(controlSys),
	// ############ Define Levels ############
	slOff("Software is off"),
	slReadyToSwitchOn("Drives are ready to switch on"),
	slEnabled("Drives are enabled and ready to move"),
	slMoving("System is moving"),
	slFault("At least one drive is in fault state"),
// 	slEmergency("Emergency state"),
// 	slSystemOn("System is ready, power off"),
// 	slStartingControl("System is starting control system"),
// 	slStoppingControl("System is stopping control system"),
// 	slPowerOn("Power is on, motors are controlled"),
	
	initDrives("Initializing drives"),
	enableDrives("Enabeling drives"),
	startMoving("Starting to move"),
	disableDrives("Disabeling drives"),
	switchOff("Switching off system"),
	restartDrives("Restarting drives"),
	recoverFromFault("Recovering from drive Fault")

// 	doSystemOn("Switch System on"),
// 	doSystemOff("Switch System off"),
// 	startControl("Start Control"),
// 	stopControl("Stop Control"),
// 	startControlDone("Control started"),
// 	stopControlDone("Control stopped"),
// 	startMoving("Start moving"),
// 	stopMoving("Stop moving"),
// 	doEmergency("Emergency"),
// 	resetEmergency("Reset emergency"),
// 	abort("abort")
	{
// 	HAL& hal = HAL::instance();

	// ############ Define critical outputs ############
// 	enable = hal.getLogicOutput("enable");
	
// 	criticalOutputs = { enable };
	
	// ############ Define critical inputs ############
// 	emergency = hal.getLogicInput("emergency");
// 	ready = hal.getLogicInput("readySig1");
	
// 	criticalInputs = { emergency, ready };
	
	addLevel(slOff);
	addLevel(slReadyToSwitchOn);
	addLevel(slEnabled);
	addLevel(slMoving);
	addLevel(slMoving);
	addLevel(slFault);
	
	slOff			.addEvent(initDrives,		slReadyToSwitchOn,	kPublicEvent  );
	slFault		.addEvent(recoverFromFault,	slReadyToSwitchOn,	kPublicEvent  );
	slReadyToSwitchOn	.addEvent(enableDrives,		slEnabled,			kPublicEvent  );
	slReadyToSwitchOn	.addEvent(switchOff,		slOff,			kPublicEvent  );
	slEnabled		.addEvent(startMoving,		slMoving,			kPublicEvent  );
	slEnabled		.addEvent(disableDrives,	slReadyToSwitchOn,	kPublicEvent  );
	slMoving		.addEvent(disableDrives,	slReadyToSwitchOn,	kPublicEvent  );
	
	
	// Add events to multiple levels
// 	addEventToLevelAndAbove(slMoving, doEmergency, slEmergency, kPublicEvent);
// 	addEventToLevelAndAbove(slEmergency, abort, slStoppingControl, kPublicEvent);
		
	
	// ############ Define input states and events for all levels ############
// 	slOff			.setInputActions( { ignore(emergency), ignore(ready) });
// 	slEmergency		.setInputActions( { ignore(emergency), ignore(ready) });
// 	slSystemOn		.setInputActions( { check(emergency, true , doEmergency), ignore(ready) });
// 	slStartingControl	.setInputActions( { check(emergency, true , doEmergency), ignore(ready)});
// 	slStoppingControl	.setInputActions( { check(emergency, true , doEmergency), ignore(ready) });
// 	slPowerOn		.setInputActions( { check(emergency, true , doEmergency), ignore(ready) });
// 	slMoving		.setInputActions( { check(emergency, true , doEmergency), check(ready, true, doEmergency) });
	
// 	slOff			.setOutputActions( { set(enable, false) } );;
// 	slEmergency		.setOutputActions( { set(enable, false) } );;
// 	slSystemOn		.setOutputActions( { set(enable, false) } );;
// 	slStartingControl	.setOutputActions( { set(enable, false) } );;
// 	slStoppingControl	.setOutputActions( { set(enable, false) } );;
// 	slPowerOn		.setOutputActions( { set(enable, true) } );;
// 	slMoving		.setOutputActions( { set(enable, true) } );;
	
	// Define and add level functions
	/////////////////////////////////////////////////////////////// 
	slOff.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
	slFault.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
	slReadyToSwitchOn.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
	slEnabled.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
	slMoving.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
	
// 	slSystemOn.setLevelAction([&](SafetyContext* privateContext) {
// 		controlSys.timedomain.stop();
// 		// you may want to check here for a user input
// 		privateContext->triggerEvent(startControl); 
// 	});
// 	
// 	slStartingControl.setLevelAction([&,dt](SafetyContext* privateContext) {
// 		controlSys.timedomain.start();
// 		if(slStartingControl.getNofActivations() * dt > 2){	// wait 2s
// 			privateContext->triggerEvent(startControlDone);
// 		}
// 	});
// 	
// 	slStoppingControl.setLevelAction([&](SafetyContext* privateContext) {
// 		controlSys.timedomain.stop();
// 		privateContext->triggerEvent(stopControlDone);
// 	});
// 	
// 	slPowerOn.setLevelAction([&](SafetyContext* privateContext) {
// 		if(ready->get()){	// check if drive is ready
// 			privateContext->triggerEvent(startMoving);
// 		}
// 	});
		
	// Define entry level
	setEntryLevel(slOff);
	
	exitFunction = ([&](SafetyContext* privateContext){
		privateContext->triggerEvent(disableDrives);
	});
	
}

MySafetyProperties::~MySafetyProperties() { }
