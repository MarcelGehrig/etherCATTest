#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

// #include <eeros/control/Sum.hpp>
// #include <eeros/control/D.hpp>
// #include <eeros/control/Gain.hpp>
// #include <eeros/control/Constant.hpp>
// #include <eeros/control/PeripheralInput.hpp>
// #include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/DeMux.hpp>

#include "../etherCatInterface/EtherCATInterfaceElmo.hpp"

#include "block_getEncoders.hpp"
#include "block_printNumber.hpp"

using namespace etherCATInterface;

class MyControlSystem {

public:
	MyControlSystem(double ts, EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal, Logger& log);
	~MyControlSystem();
	
// 	EtherCATInterfaceElmo& getElmoDrives() {return elmoDrives;);
// 	int getElmoDrives() {return 1;);
	
	block_getEncoders getEncoders;
	block_printNumber<double> printNumber;
	
	//TODO numberOfDrivesTotal
// 	eeros::control::DeMux<2, double> demux;
	
// 	eeros::control::Constant<> setpoint;
// 	eeros::control::PeripheralInput<double> enc;
// 	eeros::control::D<> diff1;
// 	eeros::control::Sum<2> sum1;
// 	eeros::control::Gain<> posController;
// 	eeros::control::D<> diff2;
// 	eeros::control::Sum<3> sum2;
// 	eeros::control::Gain<> speedController;
// 	eeros::control::Gain<> inertia;
// 	eeros::control::Gain<> invMotConst;
// 	eeros::control::PeripheralOutput<double> dac;
	
	void enableMonitoring();
	void disableMonitoring();

	eeros::control::TimeDomain timedomain;
	
	
private:
	EtherCATInterfaceElmo& elmoDrives;
	int numberOfDrivesTotal;
	Logger& log;
};

#endif // CONTROLSYSTEM_HPP_
