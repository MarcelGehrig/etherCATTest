#ifndef EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_
#define EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_


#include <eeros/logger/StreamLogWriter.hpp>

#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include "MainSequence.hpp"



namespace global {
	
// 	class MainSequence;
	
	eeros::logger::Logger* log;
	ecmasterlib::EtherCATMain* etherCATStack;
	EtherCATInterfaceElmo* elmoDrives;
	MyControlSystem* CS;
	MySafetyProperties* safetyProperties;
	eeros::safety::SafetySystem* SS;
	eeros::sequencer::Sequencer* sequencer;
// 	MainSequence* mainSequence;
}

#endif	// EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_