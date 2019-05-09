#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

// #include <eeros/control/Sum.hpp>
// #include <eeros/control/D.hpp>
// #include <eeros/control/Gain.hpp>
// #include <eeros/control/Constant.hpp>
// #include <eeros/control/PeripheralInput.hpp>
// #include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/DeMux.hpp>
#include <eeros/control/Mux.hpp>
#include <eeros/control/SignalChecker.hpp>

#include "../etherCatInterface/EtherCATInterfaceElmo.hpp"

#include "globalConfig.hpp"

#include "block_getEncoders.hpp"
#include "block_setElmos.hpp"
#include "block_printNumber.hpp"
#include "block_timingPerformanceTester.hpp"

using namespace etherCATInterface;

class MyControlSystem {

public:
	MyControlSystem(double ts, EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal);
	~MyControlSystem();
	
// 	EtherCATInterfaceElmo& getElmoDrives() {return elmoDrives;);
// 	int getElmoDrives() {return 1;);
	
	//TODO numberOfDrivesTotal
	// Inputs
	block_getEncoders getEncoders;
	eeros::control::DeMux<global::numberOfDrivesTotal, double> demuxEncoders;
	eeros::control::DeMux<global::numberOfDrivesTotal, double> demuxVelocities;
	eeros::control::DeMux<global::numberOfDrivesTotal, double> demuxTorque;
	eeros::control::Constant<uint32_t> constantDigitalOut;
	eeros::control::Constant<uint32_t> constantDigitalOut1;
	eeros::control::Constant<double> constantTargetTorque0;
	
	
	// Internal logic
	block_timingPerformanceTester<double> timingPerformanceTester;
	
	// SignalChecker
// 	eeros::control::SignalChecker<global::numberOfDrivesTotal, double> velocityChecker;
// 	eeros::control::SignalChecker< eeros::math::Matrix<2,1,double> > velocityChecker;
	eeros::control::SignalChecker< double > velocityChecker;
	
	
	// Outputs
	eeros::control::Mux<global::numberOfDrivesTotal, uint32_t> muxDigitalOut;
	eeros::control::Mux<global::numberOfDrivesTotal, double> muxTorqueOut;
	block_printNumber<double> printNumber;
	block_setElmos setElmos;
	
	
	// Methods
	void enableMonitoring();
	void disableMonitoring();

	eeros::control::TimeDomain timedomain;
	
	
private:
	EtherCATInterfaceElmo& elmoDrives;
	int numberOfDrivesTotal;
	Logger log;
};

#endif // CONTROLSYSTEM_HPP_
