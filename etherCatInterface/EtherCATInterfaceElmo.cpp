#include "EtherCATInterfaceElmo.hpp"

using namespace etherCATInterface;

EtherCATInterfaceElmo::EtherCATInterfaceElmo(ecmasterlib::EtherCATMain* etherCATStack): 
EtherCATInterfaceBase(etherCATStack)
{ }

// //blocking functions
// void EtherCATInterfaceBase::initElmoDrives()
// {
// 	log.trace() << "initElmoDrives()";
// 	bool allDrivesAreSwitchedOn = false;
// 	while (!allDrivesAreSwitchedOn) {
// 		allDrivesAreSwitchedOn = switchOnAllDrives();
// // 		usleep(1.1e6);	
// 	}
// 	
// 	for (int drive = 0; drive < numberOfDrivesTotal; drive++) {
// 		setModeOfOperation(drive, driveMode::profileVelocity);
// // 		setModeOfOperation(drive, driveMode::profilePosition);
// 	}
// 	log.trace() << "done";
// }



//advanced functions:
// void initElmoDrives()
// {
// // 	log.trace() << "initElmoDrives()";
// 	bool allDrivesAreSwitchedOn = false;
// 	while (!allDrivesAreSwitchedOn) {
// 		allDrivesAreSwitchedOn = switchOnAllDrives();
// 	}
// 	
// 	for (int drive = 0; drive < numberOfDrives; drive++) {
// 		setModeOfOperation(drive, driveMode::profileVelocity);
// // 		setModeOfOperation(drive, driveMode::profilePosition);
// 	}
// // 	log.trace() << "done";
// }

bool EtherCATInterfaceElmo::initAllDrives()
{
	return switchOnAllDrives();
}

bool EtherCATInterfaceElmo::recoverAllDrivesFromFault()
{
	return switchOnAllDrives();
}

bool EtherCATInterfaceElmo::switchOnAllDrives()
{
	bool allDrivesAreSwitchedOn = true;
	
	for (int driveNumber = 0; driveNumber < numberOfDrives; driveNumber++) {
		if (!switchOnDrive( driveNumber )) {
			allDrivesAreSwitchedOn = false;
		}
	}
	
	return allDrivesAreSwitchedOn;
}


bool EtherCATInterfaceElmo::switchOnDrive(int driveNumber)
{
	driveStatus_ELMO  driveState = getDriveStatusElmo(driveNumber);
// 			log.trace() << "drive " << driveNumber << " hase state: 0x" << std::hex << getStatus(driveNumber);
	
	// 0:	'start' -> 'not ready to switch on'					: The drive self-tests and/or self-initializes
	
	// 1:	'not ready to switch on' -> 'switch on disabled'	: The drive activates communication
	
	// 15: 	fault reset
// 	if (checkMaskedBits( driveState, faultValue, faultMask)) {
	// TODO switch case
	if ( driveState == driveStatus_ELMO::fault ) {
		setControlWord(driveNumber, faultReset);
// 				log.trace() << "drive " << driveNumber << " is in 15 fault reset";
	}
	
	
	// 2:	'switch on disabled' -> 'ready to switch on'		:
// 	if (checkMaskedBits( driveState, switchOnDisabledValue, switchOnDisabledMask)) {
	if ( driveState == driveStatus_ELMO::switchOnDisabled ) {
		setControlWord(driveNumber, shutdown);
// 				log.trace() << "drive " << driveNumber << " is in 2: switch on disabled";
	}
	
	// 3:	'ready to switch on' -> 'switched on'				:
// 	if (checkMaskedBits( driveState, readyToSwitchOnValue, readyToSwitchOnMask)) {
	if ( driveState == driveStatus_ELMO::readyToSwitchOn ) {
		setControlWord(driveNumber, switchOn);	
// 				log.trace() << "drive " << driveNumber << " is in 3: ready to switch on";
	}
	
	return getIsDriveSwitchedOn( driveNumber) ;
}


//advanced get functions:

bool EtherCATInterfaceElmo::isAllDrivesReady()
{
	bool ready = true;
	for ( int i; i < numberOfDrives; i++) {
		if ( !getIsDriveReady(i) ) ready = false;
	}
	return ready;		
}


bool EtherCATInterfaceElmo::isAllDrivesEnabled()
{
	bool enabled = true;
	for ( int i; i < numberOfDrives; i++) {
		if ( !getIsDriveEnabled(i) ) enabled = false;
	}
	return enabled;
}

int64_t EtherCATInterfaceElmo::getPosition(int driveNumber)
{
	int32_t rawPos = ll_getPositionActualValue(driveNumber);
	int32_t diff = rawPos - drives[driveNumber].prevRawPos;
	drives[driveNumber].prevRawPos = rawPos;
	drives[driveNumber].absPos += static_cast<int64_t>(diff);
 	return drives[driveNumber].absPos + static_cast<int64_t>(drives[driveNumber].posOffset);
}

int64_t EtherCATInterfaceElmo::getPositionAux(int driveNumber)
{
	int32_t rawAuxPos = ll_getAuxilaryPositionActualValue(driveNumber);
	int32_t diff = rawAuxPos - drives[driveNumber].prevRawAuxPos;
	drives[driveNumber].prevRawAuxPos = rawAuxPos;
	drives[driveNumber].absAuxPos += static_cast<int64_t>(diff);
 	return drives[driveNumber].absAuxPos + static_cast<int64_t>(drives[driveNumber].auxPosOffset);
}



//advanced set functions:
void EtherCATInterfaceElmo::setControlWord(int driveNumber, controlWordCommand_ELMO word)
{
	switch(word) {
		case controlWordCommand_ELMO::shutdown :			ll_setControlWord(driveNumber, cwc_shutdown);
			break;
		case controlWordCommand_ELMO::switchOn :			ll_setControlWord(driveNumber, cwc_switchOn);
			break;
		case controlWordCommand_ELMO::switchOnAndEnable :	ll_setControlWord(driveNumber, cwc_switchOnAndEnable);
			break;
		case controlWordCommand_ELMO::disableVoltage :		ll_setControlWord(driveNumber, cwc_switchOnAndEnable);
			break;
		case controlWordCommand_ELMO::quickStop :			ll_setControlWord(driveNumber, cwc_quickStop);
			break;
		case controlWordCommand_ELMO::disableOperation :	ll_setControlWord(driveNumber, cwc_disableOperation);
			break;
		case controlWordCommand_ELMO::enableOperation :		ll_setControlWord(driveNumber, cwc_enableOperation);
			break;
		case controlWordCommand_ELMO::faultReset :			ll_setControlWord(driveNumber, cwc_faultReset);
			break;
		default :
			break;
	}
	
}




void EtherCATInterfaceElmo::enableAllDrives()
{
	for ( int i; i < numberOfDrives; i++) {
		enableDrive(i);
	}
}

void EtherCATInterfaceElmo::disableAllDrives()
{
	for ( int i; i < numberOfDrives; i++) {
		disableDrive(i);
	}
}



// gain scheduling functions (chair)

void EtherCATInterfaceElmo::disableVelocityControl(int driveNumber)
{
	ll_setGainSchedulingManualIndex(driveNumber, 2);
}

void EtherCATInterfaceElmo::enableVelocityControl(int driveNumber)
{
	ll_setGainSchedulingManualIndex(driveNumber, 1);
}





// basic functions

void EtherCATInterfaceElmo::disableDrive(int driveNumber)
{
	setControlWord(driveNumber, controlWordCommand_ELMO::disableOperation);
}

void EtherCATInterfaceElmo::enableDrive(int driveNumber)
{
	if ( !checkDriveStatus(driveNumber, driveStatus_ELMO::switchedOn) and getIsDriveEnabled(driveNumber) ) {
//		TODO log
// 		std::string s;
// 		s << "EnableDrive(" << driveNumber << ") with status: 0x" << std::hex << getStatusWord(driveNumber) << "not possible. It needs to be in state 'switched on'";
// 		logError(s);
	}
	else ll_setControlWord(driveNumber, cwc_enableOperation);
}

void EtherCATInterfaceElmo::setModeOfOperation(int driveNumber, driveModeOfOperation_ELMO mode, bool scheduleGainIndex)
{
	int index;
	switch(mode) {
		case driveModeOfOperation_ELMO::CANEncoderMode :
			ll_setModeOfOperation(driveNumber, dmoov_CANEncoderMode);
			break;
		case driveModeOfOperation_ELMO::profilePosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			ll_setModeOfOperation(driveNumber, dmoov_profilePosition); 
			break;
		case driveModeOfOperation_ELMO::profileVelocity :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_velocity >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_velocity);
			}
			ll_setModeOfOperation(driveNumber, dmoov_profileVelocity); 
			break;
		case driveModeOfOperation_ELMO::profileTorque :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_torque >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_torque);
			}
			ll_setModeOfOperation(driveNumber, dmoov_profileTorque); 
			break;
		case driveModeOfOperation_ELMO::homing :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_homing >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_homing);
			}
			ll_setModeOfOperation(driveNumber, dmoov_homing); 
			break;
		case driveModeOfOperation_ELMO::interpolatedPosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			ll_setModeOfOperation(driveNumber, dmoov_interpolatedPosition); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousPosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			ll_setModeOfOperation(driveNumber, dmoov_cyclicSynchronousPosition); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousVelocity :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_velocity >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_velocity);
			}
			ll_setModeOfOperation(driveNumber, dmoov_cyclicSynchronousVelocity); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousTorque :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_torque >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_torque);
			}
			ll_setModeOfOperation(driveNumber, dmoov_cyclicSynchronousTorque); 
			break;
		default : 
			break;
	}
}


bool EtherCATInterfaceElmo::getIsDriveEnabled(int driveNumber)
{
	return checkMaskedBits(ll_getStatusWord(driveNumber), operationEnabledValue, operationEnabledMask);
}

bool EtherCATInterfaceElmo::getIsDriveReady(int driveNumber)
{
	return getIsDriveSwitchedOn( driveNumber );
}

bool EtherCATInterfaceElmo::getIsDriveSwitchedOn(int driveNumber)
{
	return checkMaskedBits( ll_getStatusWord(driveNumber), switchedOnValue, switchedOnMask);
}

driveModeOfOperation_ELMO EtherCATInterfaceElmo::getDriveModeElmo(int driveNumber)
{
	int8_t modeOfOperation = ll_getModeOfOperationDisplay(driveNumber);
	switch( modeOfOperation ) {
		case dmoov_CANEncoderMode:				return driveModeOfOperation_ELMO::CANEncoderMode;
		case dmoov_profilePosition:				return driveModeOfOperation_ELMO::profilePosition;
		case dmoov_profileVelocity:				return driveModeOfOperation_ELMO::profileVelocity;
		case dmoov_profileTorque:				return driveModeOfOperation_ELMO::profileTorque;
		case dmoov_homing:						return driveModeOfOperation_ELMO::homing;
		case dmoov_interpolatedPosition:		return driveModeOfOperation_ELMO::interpolatedPosition;
		case dmoov_cyclicSynchronousPosition:	return driveModeOfOperation_ELMO::interpolatedPosition;
		case dmoov_cyclicSynchronousVelocity:	return driveModeOfOperation_ELMO::cyclicSynchronousVelocity;
		case dmoov_cyclicSynchronousTorque:		return driveModeOfOperation_ELMO::cyclicSynchronousTorque;
//		TODO log
// 		default:		std::string s;
// 						s << "EtherCATInterfaceElmo: 0x" << std::hex << modeOfOperation  << std::dec << " is no valid mode of operation code";
// 						logError(s);
	}
}

bool EtherCATInterfaceElmo::checkDriveStatus(int driveNumber, driveStatus_ELMO driveStatusToCheck)
{
	return getDriveStatusElmo(driveNumber) == driveStatusToCheck;
}


driveStatus_ELMO EtherCATInterfaceElmo::getDriveStatusElmo(int driveNumber)
{
	uint16_t statusWord = ll_getStatusWord(driveNumber);
	if ( checkMaskedBits(statusWord, notReadyToSwitchOnValue, notReadyToSwitchOnMask) ) {
		return driveStatus_ELMO::notReadyToSwitchOn;
	}
	if ( checkMaskedBits(statusWord, switchOnDisabledValue, switchOnDisabledMask) ) {
		return driveStatus_ELMO::switchOnDisabled;
	}
	if ( checkMaskedBits(statusWord, readyToSwitchOnValue, readyToSwitchOnMask) ) {
		return driveStatus_ELMO::readyToSwitchOn;
	}
	if ( checkMaskedBits(statusWord, switchedOnValue, switchedOnMask) ) {
		return driveStatus_ELMO::switchedOn;
	}
	if ( checkMaskedBits(statusWord, operationEnabledValue, operationEnabledMask) ) {
		return driveStatus_ELMO::operationEnabled;
	}
	if ( checkMaskedBits(statusWord, quickStopActiveValue, quickStopActiveMask) ) {
		return driveStatus_ELMO::quickStopActive;
	}
	if ( checkMaskedBits(statusWord, faultReactionActiveValue, faultReactionActiveMask) ) {
		return driveStatus_ELMO::faultReactionactive;
	}
	if ( checkMaskedBits(statusWord, faultValue, faultMask) ) {
		return driveStatus_ELMO::fault;
	}
	
//	TODO log
// 	std::string s;
// 	s << "EtherCATInterfaceElmo: 0x" << std::hex << statusWord  << std::dec << " is no valid status word";
// 	logError(s);


}










// index pulse
// void homeWithIndexPulse(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > offsets, bool auxPos)
// {
// 	return homeWithIndexPulse(driveNumbers, offsets, drives[drive);
// }
// 
// 
// void homeWithIndexPulse(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > offsets, bool auxPos, std::array< int, numberOfWheels > touchProbes)
// {
// 	if ( (driveNumbers.size()!=offsets.size()) || (offsets.size()!=touchProbes.size()) ) {
// 		log.error() << "homeWithIndexPulse(...) all argument lists have to be of the same size";
// 		return;
// 	}
// 	
// 	//enable capturing for all drives
// 	enableCapturingIndexPulse(driveNumbers, touchProbes);
// 	
// 	//waiting for all pulses to be captured
// 	waitForAllIndexPulses(driveNumbers, touchProbes);
// 	
// 	//set offsets
// 	setOffsetAtIndexPos(driveNumbers, offsets, touchProbes);
// }
// 
// int32_t homeWithIndexPulse(int driveNumber, int offset, bool auxPos, int touchProbe)
// {
// 	//TODO check status of drive?
// 	enableCapturingIndexPulse(driveNumber, touchProbe);
// 	while ( !getIndexPulseIsCaptured(driveNumber, touchProbe) )	usleep(100);
// 	int32_t posOffset = static_cast<int32_t>( static_cast<int>(getCapturedPosition(driveNumber)) + offset );
// 	setPosOffset(driveNumber, posOffset);asdf	//	isAuxPos ? setPosAuxOffset(driveNumber, offs) : setPosOffset(driveNumber, offs);
// 	return posOffset;
// }

bool EtherCATInterfaceElmo::enableCapturingIndexPulse(std::vector< int > driveNumbers )
{
	std::vector< int > touchProbes;
	touchProbes.resize( driveNumbers.size() );
	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
	return enableCapturingIndexPulse(driveNumbers, touchProbes);

}

bool EtherCATInterfaceElmo::enableCapturingIndexPulse(std::vector< int > driveNumbers, std::vector< int > touchProbes)
{
	if ( (driveNumbers.size()!=touchProbes.size()) ) {
		std::cout << "enableCapturingIndexPulse(...) all argument vectors have to be of the same size" << std::endl;
		return false;
	}
	
	bool allProbesReady = true;
	for (	std::vector< int >::iterator	itDriveNumbers=driveNumbers.begin(),
											itTouchProbes=touchProbes.begin();
			itDriveNumbers != driveNumbers.end();
			++itDriveNumbers, ++itTouchProbes)
	{
			if ( !enableCapturingIndexPulse(*itDriveNumbers, *itTouchProbes) ) {
				allProbesReady = false;
			}
	}
	
	return allProbesReady;
}

bool EtherCATInterfaceElmo::enableCapturingIndexPulse(int driveNumber, int touchProbe)
{
		std::cout << "enableCapturingIndexPulse is not implemented" << std::endl;
		
/*	if( touchProbe == 1 ) setTouchProbeFunction(driveNumber, touchProbeFunctionEnum_ELMO::etherCATInterface::enableSamplingAtThePositiveEdgeOfTouchProbe1)*/;
// 	if( touchProbe == 2 ) setTouchProbeFunction(driveNumber, touchProbeFunctionEnum_ELMO::switchOffTouchProbe2);
		
// 	if ( touchProbe != 1 ) {
// 		std::cout << "touchProbe " << touchProbe << "is not implemented" << std::endl;
// 		return false;
// 	}
// 	
// 	
// 	switch(touchProbeState[driveNumber]) {
// 		case touchProbeStateEnum::reset :
// 			setTouchProbeFunction( driveNumber, 0x0000 );		// disable everything
// 			if ( getTouchProbeStatus(driveNumber) == 0x0000 )	// touch probe is swithced off  -> stored values on elmo are reset
// 				touchProbeState[driveNumber] = touchProbeStateEnum::enableProbe;
// 			return false;
// 			break;
// 		case touchProbeStateEnum::enableProbe :
// 			setTouchProbeFunction( driveNumber, 0x0001 );		//0x01 enable touchProbe1; disabled sampling -> stored value on elmo is reset
// 			if ( getTouchProbeStatus(driveNumber) == 0x0001 )	// touch probe is enabled again
// 				touchProbeState[driveNumber] = touchProbeStateEnum::enableSampling;
// 			return false;
// 			break;
// 		case touchProbeStateEnum::enableSampling :
// 			setTouchProbeFunction( driveNumber, 0x0031 );		//0x31 enable sampling on positive and negative edge
// 			touchProbeState[driveNumber] = touchProbeStateEnum::enabled;
// 			return true;
// 			break;
// 		case touchProbeStateEnum::enabled :
// 			return true;
// 			break;
// 		default :
// 			log.error() << "touchProbeStateEnum does not exist";
// 	}
}


bool EtherCATInterfaceElmo::disableCapturingIndexPulse(int driveNumber, int touchProbe)
{
	if( touchProbe == 1 ) setTouchProbeFunction(driveNumber, touchProbeFunctionEnum_ELMO::switchOffTouchProbe1);
	if( touchProbe == 2 ) setTouchProbeFunction(driveNumber, touchProbeFunctionEnum_ELMO::switchOffTouchProbe2);
	
// 	if ( touchProbe != 1 ) {
// 		log.error() << "touchProbe " << touchProbe << "is not implemented";
// 		return false;
// 	}
// 	
// 	setTouchProbeFunction( driveNumber, 0x0000 );		// disable everything
// 	if ( getTouchProbeStatus(driveNumber) == 0x0000 ) {	// touch probe is swithced off  -> stored values on elmo are reset 
// 		touchProbeState[driveNumber] = touchProbeStateEnum::reset;
// 		return true;
// 	}
// 	return false;
}


// void waitForAllIndexPulses(std::array< int, numberOfDrives > driveNumbers, int pollingTimeUSec)
// {
// 	std::array< int, numberOfDrives > touchProbes;
// // 	touchProbes.resize( driveNumbers.size() );
// 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// 	return waitForAllIndexPulses(driveNumbers, touchProbes);
// 
// }
// 
// void waitForAllIndexPulses(std::array< int, numberOfDrives > driveNumbers, std::array< int, numberOfDrives > touchProbes, int pollingTimeUSec)
// {	
// 	bool allPulsesCaptured = false;
// 	while ( !allPulsesCaptured ) {
// 			for (	std::array< int, numberOfDrives >::iterator itDriveNumbers=driveNumbers.begin(),
// 					itTouchProbes=touchProbes.begin();
// 					itDriveNumbers != driveNumbers.end();
// 					++itDriveNumbers, ++itTouchProbes) {
// 						if ( getIndexPulseIsCaptured(*itDriveNumbers, *itTouchProbes) ) {
// 							allPulsesCaptured = true;
// 						}
// 						else {
// 							allPulsesCaptured = false;
// 							usleep(pollingTimeUSec);		// 0.1 sec
// 							break;
// 						}
// 					}
// 	}
// 
// }
// 
// void EtherCATInterfaceElmo::waitForAllIndexPulses(int driveNumber, int touchProbe, int pollingTimeUSec)
// {
// 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) usleep(pollingTimeUSec);
// }
// 
// 
// void setOffsetAtIndexPos(std::vector< int > driveNumbers, bool isAuxPos, std::vector< int > offsets)
// {
// 	std::vector< int > touchProbes;
// 	touchProbes.resize( driveNumbers.size() );
// 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// 	return setOffsetAtIndexPos(driveNumbers, isAuxPos, offsets, touchProbes);
// 
// }
// 
// void setOffsetAtIndexPos(std::vector< int > driveNumbers, bool isAuxPos, std::vector< int > offsets, std::vector< int > touchProbes)
// {
// 	for (	std::vector< int >::iterator itDriveNumbers=driveNumbers.begin(),
// 			itOffsets=offsets.begin(),
// 			itTouchProbes=touchProbes.begin();
// 			itDriveNumbers != driveNumbers.end();
// 			++itDriveNumbers, ++itOffsets, ++itTouchProbes) {
// 				setOffsetAtIndexPos(*itDriveNumbers, isAuxPos, *itOffsets, *itTouchProbes);
// 	}
// 
// }
// 
// void setOffsetAtIndexPos(int driveNumber, bool isAuxPos, int offset, int touchProbe)
// {
// 	log.trace() << "Argument offset: " << offset;
// 	log.trace() << "captured Position: " << getCapturedPosition(driveNumber, touchProbe);
// 	int32_t offs = static_cast<int32_t>( - static_cast<int>( getCapturedPosition(driveNumber, touchProbe) ) + offset );
// 	log.trace() << "setOffsetAtIndexPos(...) offs: " << offs;
// 	isAuxPos ? setPosAuxOffset(driveNumber, offs) : setPosOffset(driveNumber, offs);
// }


void EtherCATInterfaceElmo::setTouchProbeFunction(int driveNumber, touchProbeFunctionEnum_ELMO function)
{
	uint16_t valueToSet = drives[driveNumber].touchProbeFunctionSet;
// 	uint16_t valueToSet = 0x0;
	switch ( function ) {
		case touchProbeFunctionEnum_ELMO::switchOffTouchProbe1:
			valueToSet &= ~0x1;
			break;
		case touchProbeFunctionEnum_ELMO::enableTouchProbe1:
			valueToSet |= 0x1;
			break;
		case touchProbeFunctionEnum_ELMO::triggerWithTouchProbe1Input:
			valueToSet &= ~(0x1 << 2);
			valueToSet &= ~(0x1 << 3);
			break;
		case touchProbeFunctionEnum_ELMO::triggerWitchZeroImpulsSignalOrPositioinEncoder1:
			valueToSet |=  (0x1 << 2);
			valueToSet &= ~(0x1 << 3);
			break;
		case touchProbeFunctionEnum_ELMO::touchProbeSourceAsDefinedInObject0x60D0SI01:
			valueToSet &= ~(0x1 << 2);
			valueToSet |=  (0x1 << 3);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtThePositiveEdgeOfTouchProbe1:
			valueToSet &= ~(0x1 << 4);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe1:
			valueToSet |=  (0x1 << 4);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtTheNegativeEdgeOfTouchProbe1:
			valueToSet &= ~(0x1 << 5);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtTheNegativeEdgeOfTouchProbe1:
			valueToSet |=  (0x1 << 5);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffTouchProbe2:
			valueToSet &= ~(0x1 << 8);
			break;
		case touchProbeFunctionEnum_ELMO::enableTouchProbe2:
			valueToSet |=  (0x1 << 8);
			break;
		case touchProbeFunctionEnum_ELMO::triggerWithTouchProbe2Input:
			valueToSet &= ~(0x1 << 10);
			valueToSet &= ~(0x1 << 11);
			break;
		case touchProbeFunctionEnum_ELMO::triggerWitchZeroImpulsSignalOrPositioinEncoder2:
			valueToSet |=  (0x1 << 10);
			valueToSet &= ~(0x1 << 11);
			break;
		case touchProbeFunctionEnum_ELMO::touchProbeSourceAsDefinedInObject0x60D0SI02:
			valueToSet &= ~(0x1 << 10);
			valueToSet |=  (0x1 << 11);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtThePositiveEdgeOfTouchProbe2:
			valueToSet &= ~(0x1 << 12);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe2:
			valueToSet |=  (0x1 << 12);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtTheNegativeEdgeOfTouchProbe2:
			valueToSet &= ~(0x1 << 13);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtTheNegativeEdgeOfTouchProbe2:
			valueToSet |=  (0x1 << 13);
			break;
	}
	
	std::cout << "ELMO: valueSet: " << std::hex << valueToSet << std::endl;
	drives[driveNumber].touchProbeFunctionSet = valueToSet;
	ll_setTouchProbeFunction(driveNumber, valueToSet);
}



bool EtherCATInterfaceElmo::getTouchProbeIsEnabled(int driveNumber, int touchProbe)
{
// 	std::cout << "ELMO: getTouchProbeIsEnabled: driveNumber: " << driveNumber << "   touchProbe: " << touchProbe << std::endl;
	uint16_t touchProbeStatus = ll_getTouchProbeStatus(driveNumber);
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1EnabledValue, touchProbe1EnabledMask) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1EnabledValue, touchProbe1EnabledMask) ) return true;
	}
	else	std::cout << "touchProbe " << touchProbe << " does not exists." << std::endl;
	return false;
}

bool EtherCATInterfaceElmo::getIndexPulseIsCaptured(int driveNumber, int touchProbe)
{
	if ( getIndexPulsePositiveEdgeIsCaptured(driveNumber, touchProbe) || getIndexPulseNegativeEdgeIsCaptured(driveNumber, touchProbe) ) {
		return true;
	}
	
	return false;
}

bool EtherCATInterfaceElmo::getIndexPulsePositiveEdgeIsCaptured(int driveNumber, int touchProbe)
{
	uint16_t touchProbeStatus = ll_getTouchProbeStatus(driveNumber);
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1PositiveEdgeStoredValue, touchProbe1PositiveEdgeStoredMask) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe2PositiveEdgeStoredValue, touchProbe2PositiveEdgeStoredMask) ) return true;
	}
	else	std::cout << "touchProbe " << touchProbe << " does not exists." << std::endl;
	return false;
}

bool EtherCATInterfaceElmo::getIndexPulseNegativeEdgeIsCaptured(int driveNumber, int touchProbe)
{
	uint16_t touchProbeStatus = ll_getTouchProbeStatus(driveNumber);
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1NegativeEdgeStoredValue, touchProbe1NegativeEdgeStoredMask) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe2NegativeEdgeStoredValue, touchProbe2NegativeEdgeStoredMask) ) return true;
	}
	else	std::cout << "touchProbe " << touchProbe << " does not exists." << std::endl;
	return false;
}

// bool getIndexPulseIsCapturedIsValid(int driveNumber, int touchProbe)
// {
// 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) {
// 		log.warn() << "isDirectionOfRotationPositive(...) index pulse is not captured";
// 		return false;
// 	}
// 	
// 	if ( touchProbe == 1 ) {
// 		auto positiveEdgeValue = getCapturedPositionPositivePulse(driveNumber, touchProbe);
// 		auto negativeEdgeValue = getCapturedPositionNegativePulse(driveNumber, touchProbe);
// 		
// 		check if both physical index pulses are captured
// 		if ( abs( abs(positiveEdgeValue) - abs(negativeEdgeValue) ) < 32000 ) {	// 0/1
// 			log.trace() << "Positve edge value ( " << positiveEdgeValue << " ) is to similar to negative edge value ( " << negativeEdgeValue 
// 			<< " ). Difference: " << abs( abs(positiveEdgeValue) - abs(negativeEdgeValue) );
// 			return false;
// 		}
// 		else {
// 			return true;
// 		}
// 	}
// 	else {
// 		log.error() << "Touch probe " << touchProbe << " is not implemented.";
// 		return false;
// 	}
// }

int32_t EtherCATInterfaceElmo::getCapturedPosition(int driveNumber, int touchProbe)	// compensated for direction of rotation
{
	return getCapturedPositionPositivePulse(driveNumber, touchProbe);
}

int32_t EtherCATInterfaceElmo::getCapturedPositionPositivePulse(int driveNumber, int touchProbe)
{
	if	( touchProbe == 1 )		return ll_getTouchProbePos1Positive(driveNumber);
	else if	( touchProbe == 2 )	return ll_getTouchProbePos2Positive(driveNumber);
	else						std::cout << "touchProbe " << touchProbe << " does not exists." << std::endl;
	return 0;
}

int32_t EtherCATInterfaceElmo::getCapturedPositionNegativePulse(int driveNumber, int touchProbe)
{
	if	( touchProbe == 1 )		return ll_getTouchProbePos1Negative(driveNumber);
	else if	( touchProbe == 2 )	std::cout << "No TouchProbePosition negative pulse for TouchProbe 2." << std::endl;
	else						std::cout << "touchProbe " << touchProbe << " does not exists." << std::endl;
	return 0;
}

// bool isDirectionOfRotationPositive(int driveNumber, int touchProbe)
// {
// 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) log.warn() << "isDirectionOfRotationPositive(...) index pulse is not captured";
// 	
// 	auto positiveEdgeValue = getCapturedPositionPositivePulse(driveNumber, touchProbe);
// 	auto negativeEdgeValue = getCapturedPositionNegativePulse(driveNumber, touchProbe);
// 		
// 	if (positiveEdgeValue < negativeEdgeValue  ) {			// positive direction of rotation
// 		return true;
// 	}
// 	else if ( positiveEdgeValue > negativeEdgeValue  ) {	// negative direction of rotation
// 		return false;
// 	}
// }



	
	
// Basic get and set functions to get/write values from/to buffer array
// ////////////////////////////////////////////////////////////////////

//basic set functions:
void EtherCATInterfaceElmo::ll_setControlWord(int driveNumber, uint16_t controlWord)
{
	set16bit(oo_controlWord, driveNumber, controlWord);
}

void EtherCATInterfaceElmo::ll_setModeOfOperation(int driveNumber, int8_t modeOfOperation)
{
	set8bit(oo_modeOfOperation, driveNumber, modeOfOperation);
}

void EtherCATInterfaceElmo::ll_setTargetTorque(int driveNumber, int16_t targetTorque)
{
	set16bit(oo_targetTorque, driveNumber, targetTorque);
}

void EtherCATInterfaceElmo::ll_setMaxTorque(int driveNumber, int16_t maxTorque)
{
	set16bit(oo_maxTorque, driveNumber, maxTorque);
}

void EtherCATInterfaceElmo::ll_setMaxCurrent(int driveNumber, int16_t maxCurrent)
{
	set16bit(oo_maxCurrent, driveNumber, maxCurrent);
}

void EtherCATInterfaceElmo::ll_setTargetPosition(int driveNumber, int32_t targetPosition)
{
	set32bit(oo_targetPostition, driveNumber, targetPosition);
}

void EtherCATInterfaceElmo::ll_setMaxProfileVelocity(int driveNumber, uint32_t maxProfileVelocity)
{
	set32bit(oo_maxProfileVelocity, driveNumber, maxProfileVelocity);
}

void EtherCATInterfaceElmo::ll_setProfileVelocity(int driveNumber, uint32_t profileVelocity)
{
	set32bit(oo_profileVelocity, driveNumber, profileVelocity);
}

void EtherCATInterfaceElmo::ll_setEndVelocity(int driveNumber, uint32_t endVelocity)
{
	set32bit(oo_endVelocity, driveNumber, endVelocity);
}

void EtherCATInterfaceElmo::ll_setProfileAcceleration(int driveNumber, uint32_t profileAcceleration)
{
	set32bit(oo_profileAcceleration, driveNumber, profileAcceleration);
}

void EtherCATInterfaceElmo::ll_setProfileDeceleration(int driveNumber, uint32_t profileDeceleration)
{
	set32bit(oo_profileDeceleration, driveNumber, profileDeceleration);
}

void EtherCATInterfaceElmo::ll_setTorqueSlope(int driveNumber, uint32_t torqueSlope)
{
	set32bit(oo_torqueSlope, driveNumber, torqueSlope);
}

void EtherCATInterfaceElmo::ll_setPositionOffset(int driveNumber, int32_t positionOffset)
{
	set32bit(oo_positionOffset, driveNumber, positionOffset);
}

void EtherCATInterfaceElmo::ll_setVelocityOffset(int driveNumber, int32_t velocityOffset)
{
	set32bit(oo_velocityOffset, driveNumber, velocityOffset);
}

void EtherCATInterfaceElmo::ll_setTorqueOffset(int driveNumber, int16_t torqueOffset)
{
	set16bit(oo_torqueOffset, driveNumber, torqueOffset);
}

void EtherCATInterfaceElmo::ll_setTouchProbeFunction(int driveNumber, uint16_t touchProbeFunction)
{
// 	std::cout << "Elmo: setTouchProbeFunction: driveNumber: " << driveNumber << "   value: 0x" << std::hex << touchProbeFunction << std::endl;
	set16bit(oo_touchProbeFunction, driveNumber, touchProbeFunction);
}

void EtherCATInterfaceElmo::ll_setInterpolatedDataRecord1(int driveNumber, int32_t interpolatedDataRecord1)
{
	set32bit(oo_interpolatedDataRecord_1, driveNumber, interpolatedDataRecord1);
}

void EtherCATInterfaceElmo::ll_setInterpolatedDataRecord2(int driveNumber, int32_t interpolatedDataRecord2)
{
	set32bit(oo_interpolatedDataRecord_2, driveNumber, interpolatedDataRecord2);
}

void EtherCATInterfaceElmo::ll_setTargetVelocity(int driveNumber, int32_t targetVelocity)
{
	set32bit(oo_targetVelocity, driveNumber, targetVelocity);
}

void EtherCATInterfaceElmo::ll_setDigitalOutput(int driveNumber, uint32_t digitalOutput)
{
	set32bit(oo_digitalOutput, driveNumber, digitalOutput);
}

void EtherCATInterfaceElmo::ll_setPolarity(int driveNumber, uint8_t polarity)
{
	set8bit(oo_polarity, driveNumber, polarity);
}

void EtherCATInterfaceElmo::ll_setGainSchedulingManualIndex(int driveNumber, uint16_t index)
{
	set16bit(oo_gainSchedlingManualIndex, driveNumber, index);
}



//TODO casting signed/unsigned?

//basic get functions:
uint16_t EtherCATInterfaceElmo::ll_getStatusWord(int driveNumber)
{
	return (uint16_t )get16bit(io_statusWord, driveNumber);
}

int8_t EtherCATInterfaceElmo::ll_getModeOfOperationDisplay(int driveNumber)
{
	return (int8_t)get8bit(io_modeOfOperationDisplay, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionDemand_UU(int driveNumber)
{
	return (int32_t)get32bit(io_postionDemand_UU, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getActualPosition_counts(int driveNumber)
{
	return (int32_t)get32bit(io_actualPosition_counts, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_positionActualValue, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getVelocitySensorActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_velocitySensorActualValue, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getVelocityDemand(int driveNumber)
{
	return (int32_t)get32bit(io_velocityDemand, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getVelocityActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_velocityActualValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::ll_getTorqueDemandValue(int driveNumber)
{
	return (int16_t)get16bit(io_torqueDemandValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::ll_getTorqueActualValue(int driveNumber)
{
	return (int16_t)get16bit(io_torqueActualValue, driveNumber);
}

uint16_t EtherCATInterfaceElmo::ll_getTouchProbeStatus(int driveNumber)
{
	return (uint16_t)get16bit(io_touchProbeStatus, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getTouchProbePos1Positive(int driveNumber)
{
	return (int32_t)get32bit(io_touchProbePos1Positive, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getTouchProbePos1Negative(int driveNumber)
{
	return (int32_t)get32bit(io_touchProbePos1Negative, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getTouchProbePos2Positive(int driveNumber)
{
	return (int32_t)get32bit(io_touchProbePos2Positive, driveNumber);
}

uint32_t EtherCATInterfaceElmo::ll_getDCLinkCircuitVoltage(int driveNumber)
{
	return (uint32_t)get32bit(io_DCLinkCircuitVoltage, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionFollowingError(int driveNumber)
{
	return (int32_t)get32bit(io_positionFollowingError, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getControlEffort(int driveNumber)
{
	return (int32_t)get32bit(io_controlEffort, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionDemandValue_cnt(int driveNumber)
{
	return (int32_t)get32bit(io_positionDemandValue_cnt, driveNumber);
}

uint32_t EtherCATInterfaceElmo::ll_getDigitalInputs(int driveNumber)
{
	return (uint32_t)get32bit(io_digitalInputs, driveNumber);;
}

int16_t EtherCATInterfaceElmo::ll_getAnalogInput(int driveNumber)
{
	return (int16_t)get16bit(io_analogInput, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getAuxilaryPositionActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_auxilaryPositionActualValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::ll_getCurrentActualValue(int driveNumber)
{
	return (int16_t)get16bit(io_currentActualValue, driveNumber);
}



