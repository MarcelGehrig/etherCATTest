#include "MyControlSystem.hpp"
#include <eeros/core/Executor.hpp>
#include <float.h>

using namespace eeros::control;

MyControlSystem::MyControlSystem(double ts, EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal, Logger& log) :
log(log),
elmoDrives(elmoDrives),
numberOfDrivesTotal(numberOfDrivesTotal),
getEncoders(elmoDrives, numberOfDrivesTotal),
demuxEncoders(),
//TODO min max value
// 	positionChecker(-DBL_MAX, 10000),
positionChecker(-10, 10000),
constantDigitalOut(0),
constantDigitalOut1(0),
printNumber(log, "Encoder: ", "", 200, false),
setElmos(elmoDrives, numberOfDrivesTotal),
timedomain("Main time domain", ts, true) 
{
	muxDigitalOut.setName("muxDigitalOut");
	// Inputs
	demuxEncoders.getIn().connect(getEncoders.getOutPosition());
	muxDigitalOut.getIn(0).connect(constantDigitalOut.getOut());
	muxDigitalOut.getIn(1).connect(constantDigitalOut1.getOut());
	
	// Internal logic
	
	// Signal checker
	positionChecker.setName("signal checker position");
	positionChecker.getIn().connect(demuxEncoders.getOut(0));
	
	// Outputs
	printNumber.getIn().connect(demuxEncoders.getOut(0));
	setElmos.getInDigitalOutput().connect(muxDigitalOut.getOut());

	// Timedomains
	timedomain.addBlock(getEncoders);
	timedomain.addBlock(demuxEncoders);
	timedomain.addBlock(constantDigitalOut);
	timedomain.addBlock(constantDigitalOut1);
	timedomain.addBlock(muxDigitalOut);
	timedomain.addBlock(positionChecker);
	timedomain.addBlock(printNumber);
	timedomain.addBlock(setElmos);
	
	eeros::Executor::instance().add(timedomain);
}

void MyControlSystem::enableMonitoring()
{
	printNumber.enable();
}

void MyControlSystem::disableMonitoring()
{
	printNumber.disable();
}


MyControlSystem::~MyControlSystem() { }
