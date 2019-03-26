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


MySafetyProperties::MySafetyProperties(MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, double dt) : 
	CS(CS),
	elmoDrives(elmoDrives),
	// ############ Define Levels ############
	slOff("Software is off"),
	slEmergency("Emergency state"),
	slDrivesDisabled("Drives are disabled"),
	slDrivesEnabled("Drives are enabled and ready to move"),
	
	switchOff("Switching off"),
	doEmergency("Emergency"),
	disableDrives("Disabeling drives"),
	enableDrives("Enabeling drives")

// 	abort("abort")
	{
// 	HAL& hal = HAL::instance();

	// ############ Define critical outputs ############
// 	safetyTorqueOff = hal.getLogicOutput("safetyTorqueOff");
	
// 	criticalOutputs = { safetyTorqueOff };
	
	// ############ Define critical inputs ############
// 	emergency = hal.getLogicInput("emergency");
// 	ready = hal.getLogicInput("readySig1");
	
// 	criticalInputs = { emergency, ready };
	
	addLevel(slOff);
	addLevel(slEmergency);
	addLevel(slDrivesDisabled);
	addLevel(slDrivesEnabled);
// 	addLevel(slMoving);
// 	addLevel(slMoving);
// 	addLevel(slFault);
	
	slDrivesDisabled	.addEvent(enableDrives,		slDrivesEnabled,		kPublicEvent  );
	slDrivesEnabled		.addEvent(disableDrives,	slDrivesDisabled,		kPublicEvent  );
	
	
	// Add events to multiple levels
	addEventToLevelAndAbove(slEmergency,		doEmergency,		slEmergency,	kPrivateEvent);
	addEventToLevelAndAbove(slEmergency,		switchOff,			slOff,			kPublicEvent);
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
	
// 	slOff			.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slEmergency		.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slSystemOn		.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slStartingControl	.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slStoppingControl	.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slPowerOn		.setOutputActions( { set(safetyTorqueOff, true) } );;
// 	slMoving		.setOutputActions( { set(safetyTorqueOff, true) } );;
	
	
	// Define and add level functions
	/////////////////////////////////////////////////////////////// 
	slOff.setLevelAction([&](SafetyContext* privateContext) {
		elmoDrives.disableAllDrives();
		Executor::stop();
	});
	
	slEmergency.setLevelAction([&](SafetyContext* privateContext) {
		//quick stop
// 		int driveNumber;
		for (int driveNumber = 0; driveNumber < numberOfDrives; driveNumber++ ) {
			elmoDrives.setControlWord(driveNumber, quickStop);
		}
	});
// 	
// 	slFault.setLevelAction([&](SafetyContext* privateContext) {
// // 		Executor::stop();
// 	});
	
	slDrivesDisabled.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
	slDrivesEnabled.setLevelAction([&](SafetyContext* privateContext) {
// 		Executor::stop();
	});
	
// 	slMoving.setLevelAction([&](SafetyContext* privateContext) {
// // 		Executor::stop();
// 	});
	
	
// 	slSystemOn.setLevelAction([&](SafetyContext* privateContext) {
// 		CS.timedomain.stop();
// 		// you may want to check here for a user input
// 		privateContext->triggerEvent(startControl); 
// 	});
// 	
// 	slStartingControl.setLevelAction([&,dt](SafetyContext* privateContext) {
// 		CS.timedomain.start();
// 		if(slStartingControl.getNofActivations() * dt > 2){	// wait 2s
// 			privateContext->triggerEvent(startControlDone);
// 		}
// 	});
// 	
// 	slStoppingControl.setLevelAction([&](SafetyContext* privateContext) {
// 		CS.timedomain.stop();
// 		privateContext->triggerEvent(stopControlDone);
// 	});
// 	
// 	slPowerOn.setLevelAction([&](SafetyContext* privateContext) {
// 		if(ready->get()){	// check if drive is ready
// 			privateContext->triggerEvent(startMoving);
// 		}
// 	});
		
	// Define entry level
	setEntryLevel(slDrivesDisabled);
	
	exitFunction = ([&](SafetyContext* privateContext){
		privateContext->triggerEvent(disableDrives);
	});
	
}

MySafetyProperties::~MySafetyProperties() { }
