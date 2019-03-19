#ifndef ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_
#define ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
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
		Step(name, sequencer, caller), cs(cs) { }
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
		Step(name, sequencer, caller) { }
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
	InitDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& safetySys, MyControlSystem& controlSys, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, sequencer, caller),
		safetySys(safetySys),
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
	SafetySystem& safetySys;
	EtherCATInterfaceElmo& elmoDrives;
	double pos;
// 	MyControlSystem& cs;
};

class showEncoder : public Step {
public:
	showEncoder(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& safetySys, MyControlSystem& controlSys, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, sequencer, caller),
		safetySys(safetySys),
		elmoDrives(elmoDrives)
	{ }
	
	int operator() (int drive) {this->drive = drive; return start();}
	
	int action() {	}
	
	bool checkExitCondition() {	//blocking and endless
		return false;
	}

private:
	SafetySystem& safetySys;
	EtherCATInterfaceElmo& elmoDrives;
	int drive;
// 	double pos;
// 	MyControlSystem& cs;
};



class MainSequence : public Sequence {
public:
// 	MainSequence(std::string name, Sequencer& seq, SafetySystem& safetySys, MySafetyProperties& safetyProp, MyControlSystem& cs, double angle) : 
// 	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& safetySys, MySafetyProperties& safetyProp, MyControlSystem& controlSys, EtherCATInterfaceElmo& elmoDrives) : 
// 					Sequence(name, seq),
// 					safetySys(safetySys),
// 					safetyProp(safetyProp),
// 					controlSys(controlSys),
// 					elmoDrives(elmoDrives),
// // 					step_initDrives("initDrives", seq, this, safetySys, controlSys, elmoDrives),
// 					move("move", seq, this, controlSys)
// 					{
// // 					step_initDrives("initDrives", seq, this, safetySys, controlSys, elmoDrives) {
// 		log.info() << "Sequence created: " << name;
// 	}
	
// 	MainSequence(std::string name, Sequencer& sequencer) :
	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& safetySys, MySafetyProperties& safetyProp, MyControlSystem& controlSys, EtherCATInterfaceElmo& elmoDrives) :  
					Sequence(name, sequencer),
					safetySys(safetySys),
					safetyProp(safetyProp),
					controlSys(controlSys),
					elmoDrives(elmoDrives),
					step_initDrives("initDrives", sequencer, this, safetySys, controlSys, elmoDrives),
					logInfo("logInfo", sequencer, this),
					move("move", sequencer, this, controlSys)
					{
// 					step_initDrives("initDrives", seq, this, safetySys, controlSys, elmoDrives) {
		log.info() << "Sequence created: " << name;
	}
	
	
	
	int action() {
// 		sleep(5);
// 		move(1);
		step_initDrives();
		logInfo("pos0: ", std::to_string(elmoDrives.getPos(0)));
		logInfo("MainSequence finished");
		sleep(1);
// 		showEncoder(1);
		
// 		while(safetySys.getCurrentLevel() < safetyProp.slMoving);
// 	
// 		angle = 0;
// 		while (Sequencer::running) {
// 			angle += 6.28 / 10;
// // 			move(angle);
// 			sleep(1);
// 			log.info() << "enc =  " << controlSys.enc.getOut().getSignal().getValue();
// 		}
	}
private:
	InitDrives step_initDrives;
	Move move;
	LogInfo logInfo;
	double angle;
	SafetySystem& safetySys;
	MyControlSystem& controlSys;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
