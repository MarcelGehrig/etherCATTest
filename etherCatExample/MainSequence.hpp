#ifndef ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_
#define ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_

#include <eeros/core/Executor.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Wait.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include "globalConfig.hpp"
#include <unistd.h>


using namespace etherCATInterface;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::logger;


class InitDrives : public Step {
public:
	InitDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) : 
		Step(name, this),
		SS(SS),
		log(log),
		elmoDrives(elmoDrives)
		{ }
		
	int action() {	
		log.info() << "Initializing drives:";
	}
	
	bool checkExitCondition() {
		if (elmoDrives.initAllDrives()) {
			log.info() << "Drives initialized";
			return true;
		}
		return false;
	}
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	double pos;
	Logger& log;
};


class HomingDrives : public Step {
public:
	HomingDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives),
		log(log),
		wait("waiting1", this)
		{
			this->resetTimeout(); 
			this->setTimeoutTime(30);
			this->setTimeoutBehavior(SequenceProp::abort);
		}
		
	int operator() (int drive) {
		this->drive = drive;
		return start();
	}
		
	int action() {
		
		log.info() << "Homing drives";
		elmoDrives.setModeOfOperation(drive, etherCATInterface::cyclicSynchronousVelocity);
		elmoDrives.ll_setTargetVelocity(0, 5000);
		
		int attempts = 0;
		while ( attempts < 10 && !elmoDrives.enableCapturingIndexPulse(0)) {
			wait(0.1);
			attempts++;
		}
	}
	
	bool checkExitCondition() {
		return elmoDrives.getIndexPulseIsCaptured(0);
	}
	
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	Logger&  log;
	
	int drive;
	Wait wait;
};



class SetVelocity : public Step {
public:
	SetVelocity(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives),
		log(log)
		{ }
		
	int operator() (int drive, double velocity) {
		this->velocity = velocity;
		this->drive = drive;
		return start();
	}

	int action() {
		log.info() << "Set drive '" << drive << "' to velocity: " << velocity;
		elmoDrives.setModeOfOperation(drive, etherCATInterface::cyclicSynchronousVelocity);
		elmoDrives.ll_setTargetVelocity(0, velocity);
	}
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	Logger&  log;
	
	int drive;
	double velocity;
};


// ////////////////////////////////////////////////////////////////////////////
// MainSequence
// ////////////////////////////////////////////////////////////////////////////

class MainSequence : public Sequence {
public:
	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) :  
					Sequence(name, sequencer),
					sequencer(sequencer),
					SS(SS),
					safetyProp(safetyProp),
					CS(CS),
					log(log),
					elmoDrives(elmoDrives),
					wait("waiting1", this),
					step_initDrives("initDrives", sequencer, this, SS, CS, elmoDrives, log),
					step_homingDrives("homingDrives", sequencer, this, SS, CS, elmoDrives, log),
					step_setVelocity("homingDrives", sequencer, this, SS, CS, elmoDrives, log)
					{
		log.info() << "Sequence created: " << name;
	}
	
	
	
	int action() {
		// Initialize signal checkers
		CS.velocityChecker.setActiveLevel(safetyProp.slDrivesDisabled);
		CS.velocityChecker.registerSafetyEvent(SS, safetyProp.doEmergency);
		CS.velocityChecker.setLimits(-100000, 100000);
// 		wait(2);
		CS.velocityChecker.reset();
		//TODO
		step_initDrives();
// 		SS.triggerEvent(safetyProp.enableDrives);
		
		SS.triggerEvent(safetyProp.enableDrives);
		step_homingDrives(0);
		elmoDrives.setOffsetAtIndexPos(0);
		log.info() << "Index pulse captured at:      " << elmoDrives.getCapturedPosition(0);
		log.info() << "getPosition(0):               " << elmoDrives.getPosition(0);
		log.info() << "ll_getPositionActualValue(0): " << elmoDrives.ll_getPositionActualValue(0);
		SS.triggerEvent(safetyProp.disableDrives);
		
		// set velocity	
// 		SS.triggerEvent(safetyProp.enableDrives);
// 		step_setVelocity(0, -5000);
// 		wait(4);
// 		step_setVelocity(0, 0);
		
		
		// timingPerformanceTest
// 		log.info() << "timingPerformanceTest started";
// 		elmoDrives.ll_setTargetTorque(0, 0);
// 		elmoDrives.setModeOfOperation(0, etherCATInterface::cyclicSynchronousTorque);
// 		SS.triggerEvent(safetyProp.enableDrives);
// 		CS.setElmos.setTargetTorqueByCS = true;
// 		CS.timingPerformanceTester.enable();
// 		wait(1);
// 		log.info() << "timingPerformanceTest without waiting";
// 		elmoDrives.getEtherCATStack()->getInstance()->stopWaitingForEeros();
// 		wait(1);
// 		CS.timingPerformanceTester.disable();
// 		CS.setElmos.setTargetTorqueByCS = false;
// 		elmoDrives.disableAllDrives();
		
		
		for(int i=0; i<30 and sequencer.running; i++) {
			log.info() << "SS level:                     " << SS.getCurrentLevel();
			log.info() << "DI:                           0x" << std::hex << CS.getEncoders.getOutDigitalInputs().getSignal().getValue();
			log.info() << "Velocity:                     " << CS.getEncoders.getOutVelocity().getSignal().getValue();
			log.info() << "Vel SC:                       " << CS.demuxVelocities.getOut(0).getSignal().getValue();
			log.info() << "getPosition(0):               " << elmoDrives.getPosition(0);
			log.info() << "ll_getPositionActualValue(0): " << elmoDrives.ll_getPositionActualValue(0);
			log.info();
			
			if ( i%2 == 0 )
				CS.constantDigitalOut.setValue(0xFFFFFFFF);
// 				CS.setElmos.getInDigitalOutput().getSignal().setValue<uint32_t>(0xFFFFFFFF);
// 				elmoDrives.ll_setDigitalOutput(0, 0xFFFFFFFF);
			else
				CS.constantDigitalOut.setValue(0x0);
// 				CS.setElmos.getInDigitalOutput().getSignal().setValue<uint32_t>(0x0);
// 				elmoDrives.ll_setDigitalOutput(0, 0x0);
				
			wait(1);
		}
		
		
// 		wait(30);
		log.info() << "MainSequence finished";
		
		SS.triggerEvent(safetyProp.switchOff);
	}
private:
	Wait wait;
	InitDrives step_initDrives;
	HomingDrives step_homingDrives;
	SetVelocity step_setVelocity;
	
	double angle;
	
	Sequencer& sequencer;
	SafetySystem& SS;
	MyControlSystem& CS;
	Logger& log;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
