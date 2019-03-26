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
		global::log->info() << "Initializing drives:";
	}
	bool checkExitCondition() {
		if (elmoDrives.initAllDrives()) {
			global::log->info() << "Drives initialized";
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



class MainSequence : public Sequence {
public:
	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) :  
					Sequence(name, sequencer),
					SS(SS),
					safetyProp(safetyProp),
					CS(CS),
					elmoDrives(elmoDrives),
					wait("waiting1", this),
					step_initDrives("initDrives", sequencer, this, SS, CS, elmoDrives)
					{
		global::log->info() << "Sequence created: " << name;
	}
	
	
	
	int action() {
		step_initDrives();
		wait(3);
		CS.enableMonitoring();
// 		global::log->info() << "pos0: " << std::to_string(elmoDrives.getPos(0));
		
		wait(3);
		global::log->info() << "MainSequence finished";
		
		SS.triggerEvent(safetyProp.switchOff);
	}
private:
	Wait wait;
	InitDrives step_initDrives;
	double angle;
	SafetySystem& SS;
	MyControlSystem& CS;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
