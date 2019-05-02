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


//#include "etherCatInterface/EtherCATInterfaceElmo.hpp"

using namespace etherCATInterface;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::logger;

class Move : public Step {
public:
	Move(std::string name, Sequencer& sequencer, BaseSequence* caller, MyControlSystem& cs) : 
		Step(name, this), cs(cs) { }
	int operator() (double pos) {this->pos = pos; return start();}
	int action() {
// 		cs.setpoint.setValue(pos);
		sleep(1);
	}
	double pos;
	MyControlSystem& cs;
};


class InitDrives : public Step {
public:
	InitDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives)
		{ }
	int action() {	
// 		log.info() << "Initializing drives:";
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
// 	MyControlSystem& cs;
};


class HomingDrives : public Step {
public:
	HomingDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives),
		wait("waiting1", this)
		{ }
		
	int action() {	
		elmoDrives.ll_setTargetTorque(0, 0);
		wait(1);
		elmoDrives.ll_setTargetTorque(0, 0);
	}
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	double pos;
	Wait wait;
// 	MyControlSystem& cs;
};

class showEncoder : public Step {
public:
	showEncoder(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives)
	{ }
	
	int operator() (int drive) {this->drive = drive; return start();}
	
	int action() {	}
	
	bool checkExitCondition() {	//blocking and endless
		return false;
	}

private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	int drive;
// 	double pos;
// 	MyControlSystem& cs;
};


// MainSequence
// ////////////////////////////////////////////////////////////////////////////
class MainSequence : public Sequence {
public:
	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) :  
// 	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) :  
					Sequence(name, sequencer),
					sequencer(sequencer),
					SS(SS),
					safetyProp(safetyProp),
					CS(CS),
					log(log),
					elmoDrives(elmoDrives),
					wait("waiting1", this),
					step_initDrives("initDrives", sequencer, this, SS, CS, elmoDrives),
					step_homingDrives("homingDrives", sequencer, this, SS, CS, elmoDrives)
					{
		log.info() << "Sequence created: " << name;
	}
	
	
	
	int action() {
		// Initialize signal checkers
		CS.velocityChecker.setActiveLevel(safetyProp.slDrivesDisabled);
		CS.velocityChecker.registerSafetyEvent(SS, safetyProp.doEmergency);
// 		wait(2);
		CS.velocityChecker.reset();
// 		SS.triggerEvent(safetyProp.recoverEmergency);
		CS.velocityChecker.setLimits(-40000, 40000);
// 		std::cout << 5 << std::endl;
		//TODO
		step_initDrives();
		step_homingDrives();
		wait(3);
// 		CS.enableMonitoring();
		
// 		global::log->info() << "pos0: " << std::to_string(elmoDrives.getPos(0));
		
		for(int i=0; i<30 and sequencer.running; i++) {
			log.info() << "SS level: " << SS.getCurrentLevel();
			log.info() << "DI: 0x" << std::hex << CS.getEncoders.getOutDigitalInputs().getSignal().getValue();
			log.info() << "Velocity:" << CS.getEncoders.getOutVelocity().getSignal().getValue();
			log.info() << "Vel SC: " << CS.demuxVelocities.getOut(0).getSignal().getValue();
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
	
	double angle;
	
	Sequencer& sequencer;
	SafetySystem& SS;
	MyControlSystem& CS;
	Logger& log;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
