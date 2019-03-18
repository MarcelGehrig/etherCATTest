#ifndef SAFETYPROPERTIES_HPP_
#define SAFETYPROPERTIES_HPP_


#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "MyControlSystem.hpp"

class MySafetyProperties : public eeros::safety::SafetyProperties {
	
public:
	MySafetyProperties(MyControlSystem& controlSys, double ts);
	virtual ~MySafetyProperties();
	
	// Name all levels
	eeros::safety::SafetyLevel slFault;
	eeros::safety::SafetyLevel slOff;
	eeros::safety::SafetyLevel slReadyToSwitchOn;
	eeros::safety::SafetyLevel slEnabled;
	eeros::safety::SafetyLevel slMoving;
	
// 	eeros::safety::SafetyLevel slEmergency;
// 	eeros::safety::SafetyLevel slSystemOn;
// 	eeros::safety::SafetyLevel slStartingControl;
	
	// Define all possible events
	eeros::safety::SafetyEvent initDrives;
	eeros::safety::SafetyEvent enableDrives;
	eeros::safety::SafetyEvent startMoving;
	eeros::safety::SafetyEvent disableDrives;
	eeros::safety::SafetyEvent switchOff;
	eeros::safety::SafetyEvent restartDrives;
	eeros::safety::SafetyEvent recoverFromFault;
	
// 	eeros::safety::SafetyEvent doSystemOn;
// 	eeros::safety::SafetyEvent doSystemOff;
// 	eeros::safety::SafetyEvent startControl;
// 	eeros::safety::SafetyEvent stopControl;
// 	eeros::safety::SafetyEvent startControlDone;
// 	eeros::safety::SafetyEvent stopControlDone;
// 	eeros::safety::SafetyEvent startMoving;
// 	eeros::safety::SafetyEvent stopMoving;
// 	eeros::safety::SafetyEvent doEmergency;
// 	eeros::safety::SafetyEvent resetEmergency;	
// 	eeros::safety::SafetyEvent abort;
	
protected:
	// critical outputs
	eeros::hal::Output<bool>* enable;
	
	// critical inputs
	eeros::hal::Input<bool>* emergency;
	eeros::hal::Input<bool>* ready;
		
	MyControlSystem& controlSys;
};

#endif // SAFETYPROPERTIES_HPP_
