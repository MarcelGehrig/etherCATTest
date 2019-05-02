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
demuxVelocities(),
//TODO min max value
// 	velocityChecker(-DBL_MAX, 10000),
// velocityChecker(-DBL_MAX, DBL_MAX),
velocityChecker(-99999999, 9999999),
constantDigitalOut(0),
constantDigitalOut1(0),
printNumber(log, "Encoder: ", "", 200, false),
setElmos(elmoDrives, numberOfDrivesTotal),
timedomain("Main time domain", ts, true) 
{
	getEncoders.setName("getEncoders");
	demuxEncoders.setName("demuxEncoders");
	demuxVelocities.setName("demuxVelocities");
	constantDigitalOut.setName("constantDigitalOut");
	constantDigitalOut1.setName("constantDigitalOut1");
	muxDigitalOut.setName("muxDigitalOut");
	velocityChecker.setName("velocityChecker");
	printNumber.setName("printNumber");
	setElmos.setName("setElmos");

	// Inputs
	demuxEncoders.getIn().connect(getEncoders.getOutPosition());
	demuxVelocities.getIn().connect(getEncoders.getOutVelocity());
	muxDigitalOut.getIn(0).connect(constantDigitalOut.getOut());
	muxDigitalOut.getIn(1).connect(constantDigitalOut1.getOut());
	
	// Internal logic
	
	// Signal checker
	velocityChecker.setName("signal checker position");
// 	velocityChecker.getIn().connect(demuxEncoders.getOut(0));
// 	velocityChecker.getIn().connect(getEncoders.getOutVelocity());
	velocityChecker.getIn().connect(demuxVelocities.getOut(0));
	
	// Outputs
	printNumber.getIn().connect(demuxEncoders.getOut(0));
	setElmos.getInDigitalOutput().connect(muxDigitalOut.getOut());

	// Timedomains
	timedomain.addBlock(getEncoders);
	timedomain.addBlock(demuxEncoders);
	timedomain.addBlock(demuxVelocities);
	timedomain.addBlock(constantDigitalOut);
	timedomain.addBlock(constantDigitalOut1);
	timedomain.addBlock(muxDigitalOut);
	timedomain.addBlock(velocityChecker);
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
