#ifndef ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_
#define ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include <unistd.h>


//#include "etherCatInterface/EtherCATInterfaceElmo.hpp"

using namespace etherCATInterface;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::logger;

class Move : public Step {
public:
	Move(std::string name, Sequencer& sequencer, BaseSequence* caller, MyControlSystem& cs) : Step(name, sequencer, caller), cs(cs) { }
	int operator() (double pos) {this->pos = pos; return Step::start();}
	int action() {
// 		cs.setpoint.setValue(pos);
		sleep(1);
	}
	double pos;
	MyControlSystem& cs;
};

class initDrives : public Step {
public:
	initDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& safetySys, MyControlSystem& controlSys, EtherCATInterfaceElmo& elmoDrives) : 
		Step(name, sequencer, caller),
		safetySys(safetySys),
		elmoDrives(elmoDrives)
		{ }
// 	int operator() (double pos) { this->pos = pos;}
	int action() {	}
	bool checkExitCondition() {
		return elmoDrives.initAllDrives();
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
	
	int operator() (int drive) {this->drive = drive;}
	
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
	
	MainSequence(std::string name, Sequencer& sequencer) : 
					Sequence(name, seq),
					safetySys(safetySys),
					safetyProp(safetyProp),
					controlSys(controlSys),
					elmoDrives(elmoDrives),
// 					step_initDrives("initDrives", seq, this, safetySys, controlSys, elmoDrives),
					move("move", seq, this, controlSys)
					{
// 					step_initDrives("initDrives", seq, this, safetySys, controlSys, elmoDrives) {
		log.info() << "Sequence created: " << name;
	}
	
	
	
	int action() {
		sleep(5);
		move(1);
// 		step_initDrives();
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
// 	initDrives step_initDrives;
	Move move;
	double angle;
	SafetySystem& safetySys;
	MyControlSystem& controlSys;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
