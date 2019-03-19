#ifndef EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_
#define EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_


#include <eeros/logger/StreamLogWriter.hpp>

#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include "MainSequence.hpp"



namespace global {
	
	eeros::logger::Logger* log;
	MyControlSystem* CS;
	
}

#endif	// EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_