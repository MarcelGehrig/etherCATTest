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

class LogInfo : public Step {
public:
	LogInfo(std::string name, Sequencer& sequencer, BaseSequence* caller) : 
		Step(name, this) { }
	int operator() (std::string message0, std::string message1="", std::string message2="", std::string message3="") {
		this->message = message0 + message1 + message2 + message3;
		return start();		
	}
	int action() {
		global::log->info() << message;
	}
	std::string message;
};

static int i3 = 0;
class InitDrives : public Step {
public:
	InitDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives)
		{ }
// 	int operator() (double pos) { this->pos = pos; return start();}
	int action() {	
		global::log->info() << "Initializing drives:";
// 		sleep (10);
// 		elmoDrives.initAllDrives();
// 		global::log->info() << "Initializing drives:";
// 		global::log->info() << "Initializing drives:" << elmoDrives.getPos(0);
// 		sleep (10);
	}
	bool checkExitCondition() {
// 		if (i3 == 1 ) return true;
// 		else i3++;
// 		global::log->info() << "Initializing drives:" << elmoDrives.getPos(0);
// 		global::log->info() << "Initializing drives:" << elmoDrives.getPos(1);
// 		return true;
		if (elmoDrives.initAllDrives()) {
			global::log->info() << "Drives initialized";
			return true;
		}
// 		if (elmoDrives.getPos(0)) {
// // 			global::log->info() << "Drives initialized";
// 			return true;
// 		}
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
// 	MainSequence(std::string name, Sequencer& seq, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& cs, double angle) : 
// 	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) : 
// 					Sequence(name, seq),
// 					SS(SS),
// 					safetyProp(safetyProp),
// 					CS(CS),
// 					elmoDrives(elmoDrives),
// // 					step_initDrives("initDrives", seq, this, SS, CS, elmoDrives),
// 					move("move", seq, this, CS)
// 					{
// // 					step_initDrives("initDrives", seq, this, SS, CS, elmoDrives) {
// 		log.info() << "Sequence created: " << name;
// 	}
	
// 	MainSequence(std::string name, Sequencer& sequencer) :
	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives) :  
					Sequence(name, sequencer),
					SS(SS),
					safetyProp(safetyProp),
					CS(CS),
					elmoDrives(elmoDrives),
					wait("waiting1", this),
					step_initDrives("initDrives", sequencer, this, SS, CS, elmoDrives),
					logInfo("logInfo", sequencer, this),
					move("move", sequencer, this, CS)
					{
// 					step_initDrives("initDrives", seq, this, SS, CS, elmoDrives) {
		log.info() << "Sequence created: " << name;
	}
	
	
	
	int action() {
// 		sleep(5);
// 		move(1);
		step_initDrives();
		global::log->info() << "pos0: " << std::to_string(elmoDrives.getPos(0));
		global::log->info() << "MainSequence finished";
		
		wait(5);
// 		sleep(1);
// 		showEncoder(1);
		
// 		while(SS.getCurrentLevel() < safetyProp.slMoving);
// 	
// 		angle = 0;
// 		while (Sequencer::running) {
// 			angle += 6.28 / 10;
// // 			move(angle);
// 			sleep(1);
// 			log.info() << "enc =  " << CS.enc.getOut().getSignal().getValue();
// 		}
// 		eeros::Executor::stop();
		SS.triggerEvent(safetyProp.switchOff);
	}
private:
	Wait wait;
	InitDrives step_initDrives;
	Move move;
	LogInfo logInfo;
	double angle;
	SafetySystem& SS;
	MyControlSystem& CS;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
