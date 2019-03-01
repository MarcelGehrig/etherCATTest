#include "EtherCATInterfaceElmo.hpp"

using namespace etherCATInterface;

EtherCATInterfaceElmo::EtherCATInterfaceElmo(ecmasterlib::EtherCATMain* etherCATStack): 
EtherCATInterfaceBase(etherCATStack)
{ }




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

int64_t EtherCATInterfaceElmo::getPos(int driveNumber)
{
	int32_t rawPos = getPositionActualValue(driveNumber);
	int32_t diff = rawPos - drives[driveNumber].prevRawPos;
	drives[driveNumber].prevRawPos = rawPos;
	drives[driveNumber].absPos += static_cast<int64_t>(diff);
 	return drives[driveNumber].absPos + static_cast<int64_t>(drives[driveNumber].posOffset);
}

int64_t EtherCATInterfaceElmo::getPosAux(int driveNumber)
{
	int32_t rawAuxPos = getAuxilaryPositionActualValue(driveNumber);
	int32_t diff = rawAuxPos - drives[driveNumber].prevRawAuxPos;
	drives[driveNumber].prevRawAuxPos = rawAuxPos;
	drives[driveNumber].absAuxPos += static_cast<int64_t>(diff);
 	return drives[driveNumber].absAuxPos + static_cast<int64_t>(drives[driveNumber].auxPosOffset);
}



//advanced set functions:
void EtherCATInterfaceElmo::setControlWord(int driveNumber, controlWordCommand_ELMO word)
{
	switch(word) {
		case controlWordCommand_ELMO::shutdown :			setControlWord(driveNumber, cwc_shutdown);
			break;
		case controlWordCommand_ELMO::switchOn :			setControlWord(driveNumber, cwc_switchOn);
			break;
		case controlWordCommand_ELMO::switchOnAndEnable :	setControlWord(driveNumber, cwc_switchOnAndEnable);
			break;
		case controlWordCommand_ELMO::disableVoltage :		setControlWord(driveNumber, cwc_switchOnAndEnable);
			break;
		case controlWordCommand_ELMO::quickStop :			setControlWord(driveNumber, cwc_quickStop);
			break;
		case controlWordCommand_ELMO::disableOperation :	setControlWord(driveNumber, cwc_disableOperation);
			break;
		case controlWordCommand_ELMO::enableOperation :		setControlWord(driveNumber, cwc_enableOperation);
			break;
		case controlWordCommand_ELMO::faultReset :			setControlWord(driveNumber, cwc_faultReset);
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
	setGainSchedulingManualIndex(driveNumber, 2);
}

void EtherCATInterfaceElmo::enableVelocityControl(int driveNumber)
{
	setGainSchedulingManualIndex(driveNumber, 1);
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
	else setControlWord(driveNumber, cwc_enableOperation);
}

void EtherCATInterfaceElmo::setModeOfOperation(int driveNumber, driveModeOfOperation_ELMO mode, bool scheduleGainIndex)
{
	int index;
	switch(mode) {
		case driveModeOfOperation_ELMO::CANEncoderMode :
			setModeOfOperation(driveNumber, dmoov_CANEncoderMode);
			break;
		case driveModeOfOperation_ELMO::profilePosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			setModeOfOperation(driveNumber, dmoov_profilePosition); 
			break;
		case driveModeOfOperation_ELMO::profileVelocity :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_velocity >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_velocity);
			}
			setModeOfOperation(driveNumber, dmoov_profileVelocity); 
			break;
		case driveModeOfOperation_ELMO::profileTorque :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_torque >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_torque);
			}
			setModeOfOperation(driveNumber, dmoov_profileTorque); 
			break;
		case driveModeOfOperation_ELMO::homing :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_homing >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_homing);
			}
			setModeOfOperation(driveNumber, dmoov_homing); 
			break;
		case driveModeOfOperation_ELMO::interpolatedPosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			setModeOfOperation(driveNumber, dmoov_interpolatedPosition); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousPosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			setModeOfOperation(driveNumber, dmoov_cyclicSynchronousPosition); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousVelocity :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_velocity >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_velocity);
			}
			setModeOfOperation(driveNumber, dmoov_cyclicSynchronousVelocity); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousTorque :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_torque >= 0) ) {
				setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_torque);
			}
			setModeOfOperation(driveNumber, dmoov_cyclicSynchronousTorque); 
			break;
		default : 
			break;
	}
}


bool EtherCATInterfaceElmo::getIsDriveEnabled(int driveNumber)
{
	return checkMaskedBits(getStatusWord(driveNumber), operationEnabledValue, operationEnabledMask);
}

bool EtherCATInterfaceElmo::getIsDriveReady(int driveNumber)
{
	return getIsDriveSwitchedOn( driveNumber );
}

bool EtherCATInterfaceElmo::getIsDriveSwitchedOn(int driveNumber)
{
	return checkMaskedBits( getStatusWord(driveNumber), switchedOnValue, switchedOnMask);
}

driveModeOfOperation_ELMO EtherCATInterfaceElmo::getDriveModeElmo(int driveNumber)
{
	int8_t modeOfOperation = getModeOfOperationDisplay(driveNumber);
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
	uint16_t statusWord = getStatusWord(driveNumber);
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










// //index pulse
// // void homeWithIndexPulse(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > offsets, bool auxPos)
// // {
// // 	std::array< int, numberOfWheels > touchProbes;
// // // 	touchProbes.resize( driveNumbers.size() );
// // 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// // 	return homeWithIndexPulse(driveNumbers, offsets, touchProbes);
// // }
// // 
// // 
// // void homeWithIndexPulse(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > offsets, bool auxPos, std::array< int, numberOfWheels > touchProbes)
// // {
// // 	if ( (driveNumbers.size()!=offsets.size()) || (offsets.size()!=touchProbes.size()) ) {
// // 		log.error() << "homeWithIndexPulse(...) all argument lists have to be of the same size";
// // 		return;
// // 	}
// // 	
// // 	//enable capturing for all drives
// // 	enableCapturingIndexPulse(driveNumbers, touchProbes);
// // 	
// // 	//waiting for all pulses to be captured
// // 	waitForAllIndexPulses(driveNumbers, touchProbes);
// // 	
// // 	//set offsets
// // 	setOffsetAtIndexPos(driveNumbers, offsets, touchProbes);
// // }
// // 
// // int32_t homeWithIndexPulse(int driveNumber, int offset, bool auxPos, int touchProbe)
// // {
// // 	//TODO check status of drive?
// // 	enableCapturingIndexPulse(driveNumber, touchProbe);
// // 	while ( !getIndexPulseIsCaptured(driveNumber, touchProbe) )	usleep(100);
// // 	int32_t posOffset = static_cast<int32_t>( static_cast<int>(getCapturedPosition(driveNumber)) + offset );
// // 	setPosOffset(driveNumber, posOffset);asdf	//	isAuxPos ? setPosAuxOffset(driveNumber, offs) : setPosOffset(driveNumber, offs);
// // 	return posOffset;
// // }
// 
// bool enableCapturingIndexPulse(std::vector< int > driveNumbers )
// {
// 	std::vector< int > touchProbes;
// 	touchProbes.resize( driveNumbers.size() );
// 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// 	return enableCapturingIndexPulse(driveNumbers, touchProbes);
// 
// }
// 
// bool enableCapturingIndexPulse(std::vector< int > driveNumbers, std::vector< int > touchProbes)
// {
// 	if ( (driveNumbers.size()!=touchProbes.size()) ) {
// 		log.error() << "enableCapturingIndexPulse(...) all argument vectors have to be of the same size";
// 		return false;
// 	}
// 	
// 	bool allProbesReady = true;
// 	for (	std::vector< int >::iterator itDriveNumbers=driveNumbers.begin(),
// 			itTouchProbes=touchProbes.begin();
// 			itDriveNumbers != driveNumbers.end();
// 			++itDriveNumbers, ++itTouchProbes)
// 	{
// 			if ( !enableCapturingIndexPulse(*itDriveNumbers, *itTouchProbes) )	allProbesReady = false;
// 	}
// 	
// 	return allProbesReady;
// }
// 
// bool enableCapturingIndexPulse(int driveNumber, int touchProbe)
// {
// 	if ( touchProbe != 1 ) {
// 		log.error() << "touchProbe " << touchProbe << "is not implemented";
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
// }
// 
// 
// bool disableCapturingIndexPulse(int driveNumber, int touchProbe)
// {
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
// }
// 
// 
// // void waitForAllIndexPulses(std::array< int, numberOfWheels > driveNumbers, int pollingTimeUSec)
// // {
// // 	std::array< int, numberOfWheels > touchProbes;
// // // 	touchProbes.resize( driveNumbers.size() );
// // 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// // 	return waitForAllIndexPulses(driveNumbers, touchProbes);
// // 
// // }
// // 
// // void waitForAllIndexPulses(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > touchProbes, int pollingTimeUSec)
// // {	
// // 	bool allPulsesCaptured = false;
// // 	while ( !allPulsesCaptured ) {
// // 			for (	std::array< int, numberOfWheels >::iterator itDriveNumbers=driveNumbers.begin(),
// // 					itTouchProbes=touchProbes.begin();
// // 					itDriveNumbers != driveNumbers.end();
// // 					++itDriveNumbers, ++itTouchProbes) {
// // 						if ( getIndexPulseIsCaptured(*itDriveNumbers, *itTouchProbes) ) {
// // 							allPulsesCaptured = true;
// // 						}
// // 						else {
// // 							allPulsesCaptured = false;
// // 							usleep(pollingTimeUSec);		// 0.1 sec
// // 							break;
// // 						}
// // 					}
// // 	}
// // 
// // }
// // 
// // void waitForAllIndexPulses(int driveNumber, int touchProbe, int pollingTimeUSec)
// // {
// // 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) usleep(pollingTimeUSec);
// // }
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
// 
// bool getIndexPulseIsCaptured(int driveNumber, int touchProbe)
// {
// 	if ( getIndexPulsePositiveEdgeIsCaptured(driveNumber, touchProbe) || getIndexPulseNegativeEdgeIsCaptured(driveNumber, touchProbe) ) {
// 		return true;
// 	}
// 	
// 	return false;
// }
// 
// bool getIndexPulsePositiveEdgeIsCaptured(int driveNumber, int touchProbe)
// {	
// 	uint16_t touchProbeStatus = getTouchProbeStatus(driveNumber);
// 	// 0x0060 both; 0x0040 negative edge; 0x0020 positive edge
// 	if	( touchProbe == 1 ) {
// 		if ( checkMaskedBits(touchProbeStatus, 0x0002, 0x0002) ) return true;
// 	}
// 	else if	( touchProbe == 2 ) {
// 		if ( checkMaskedBits(touchProbeStatus, 0x0200, 0x0200) ) return true;
// 	}
// 	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
// 	return false;
// }
// 
// bool getIndexPulseNegativeEdgeIsCaptured(int driveNumber, int touchProbe)
// {
// 	uint16_t touchProbeStatus = getTouchProbeStatus(driveNumber);
// 	// 0x0060 both; 0x0040 negative edge; 0x0020 positive edge
// 	if	( touchProbe == 1 ) {
// 		if ( checkMaskedBits(touchProbeStatus, 0x0004, 0x0004) ) return true;
// 	}
// 	else if	( touchProbe == 2 ) {
// 		if ( checkMaskedBits(touchProbeStatus, 0x0400, 0x0400) ) return true;
// 	}
// 	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
// 	return false;
// 	
// }
// 
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
// 		//check if both physical index pulses are captured
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
// 
// int32_t getCapturedPosition(int driveNumber, int touchProbe)	// compensated for direction of rotation
// {	
// 
// 	return getCapturedPositionPositivePulse(driveNumber, touchProbe);
//   
// // 	// Bastel aus Zeiten deffekter Magnetringe
// // 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) log.warn() << "isDirectionOfRotationPositive(...) index pulse is not captured";
// // 	
// // 	auto positiveEdgeValue = getCapturedPositionPositivePulse(driveNumber, touchProbe);
// // 	auto negativeEdgeValue = getCapturedPositionNegativePulse(driveNumber, touchProbe);
// // 
// // 	if (abs( abs(positiveEdgeValue) - abs(negativeEdgeValue)) < 200000) {
// // 		if (positiveEdgeValue < negativeEdgeValue  ) // positive
// // 			return getCapturedPositionPositivePulse(driveNumber, touchProbe);
// // 		else // negative
// // 			return getCapturedPositionNegativePulse(driveNumber, touchProbe);
// // 	} 
// // 	else {
// // 		if (positiveEdgeValue < negativeEdgeValue  ) // positive
// // 			return getCapturedPositionNegativePulse(driveNumber, touchProbe);
// // 		else // negative
// // 			return getCapturedPositionPositivePulse(driveNumber, touchProbe);
// // 	}
// }
// 
// int32_t getCapturedPositionPositivePulse(int driveNumber, int touchProbe)
// {
// 	int32_t position;
// 	if	( touchProbe == 1 ) position = getTouchProbePos1Positive(driveNumber);
// 	else if	( touchProbe == 2 ) position = getTouchProbePos2PosValue(driveNumber);
// 	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
// 	return position;
// }
// 
// int32_t getCapturedPositionNegativePulse(int driveNumber, int touchProbe)
// {
// 	int32_t position;
// 	if	( touchProbe == 1 ) position = getTouchProbePos1Negative(driveNumber);
// 	else if	( touchProbe == 2 ) log.error() << "No TouchProbePosition negative pulse for TouchProbe 2.";
// 	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
// 	return position;
// }
// 
// // bool isDirectionOfRotationPositive(int driveNumber, int touchProbe)
// // {
// // 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) log.warn() << "isDirectionOfRotationPositive(...) index pulse is not captured";
// // 	
// // 	auto positiveEdgeValue = getCapturedPositionPositivePulse(driveNumber, touchProbe);
// // 	auto negativeEdgeValue = getCapturedPositionNegativePulse(driveNumber, touchProbe);
// // 		
// // 	if (positiveEdgeValue < negativeEdgeValue  ) {			// positive direction of rotation
// // 		return true;
// // 	}
// // 	else if ( positiveEdgeValue > negativeEdgeValue  ) {	// negative direction of rotation
// // 		return false;
// // 	}
// // }



	
	
// Basic get and set functions to get/write values from/to buffer array
// ////////////////////////////////////////////////////////////////////

//basic set functions:
void EtherCATInterfaceElmo::setControlWord(int driveNumber, uint16_t controlWord)
{
	set16bit(oo_controlWord, driveNumber, controlWord);
}

void EtherCATInterfaceElmo::setModeOfOperation(int driveNumber, int8_t modeOfOperation)
{
	set8bit(oo_modeOfOperation, driveNumber, modeOfOperation);
}

void EtherCATInterfaceElmo::setTargetTorque(int driveNumber, int16_t targetTorque)
{
	set16bit(oo_targetTorque, driveNumber, targetTorque);
}

void EtherCATInterfaceElmo::setMaxCurrent(int driveNumber, int16_t maxCurrent)
{
	set16bit(oo_maxCurrent, driveNumber, maxCurrent);
}

void EtherCATInterfaceElmo::setTargetPosition(int driveNumber, int32_t targetPosition)
{
	set32bit(oo_targetPostition, driveNumber, targetPosition);
}

void EtherCATInterfaceElmo::setMaxProfileVelocity(int driveNumber, uint32_t maxProfileVelocity)
{
	set32bit(oo_maxProfileVelocity, driveNumber, maxProfileVelocity);
}

void EtherCATInterfaceElmo::setProfileVelocity(int driveNumber, uint32_t profileVelocity)
{
	set32bit(oo_profileVelocity, driveNumber, profileVelocity);
}

void EtherCATInterfaceElmo::setEndVelocity(int driveNumber, uint32_t endVelocity)
{
	set32bit(oo_endVelocity, driveNumber, endVelocity);
}

void EtherCATInterfaceElmo::setProfileAcceleration(int driveNumber, uint32_t profileAcceleration)
{
	set32bit(oo_profileAcceleration, driveNumber, profileAcceleration);
}

void EtherCATInterfaceElmo::setProfileDeceleration(int driveNumber, uint32_t profileDeceleration)
{
	set32bit(oo_profileDeceleration, driveNumber, profileDeceleration);
}

void EtherCATInterfaceElmo::setTorqueSlope(int driveNumber, uint32_t torqueSlope)
{
	set32bit(oo_torqueSlope, driveNumber, torqueSlope);
}

void EtherCATInterfaceElmo::setPositionOffset(int driveNumber, int32_t positionOffset)
{
	set32bit(oo_positionOffset, driveNumber, positionOffset);
}

void EtherCATInterfaceElmo::setVelocityOffset(int driveNumber, int32_t velocityOffset)
{
	set32bit(oo_velocityOffset, driveNumber, velocityOffset);
}

void EtherCATInterfaceElmo::setTorqueOffset(int driveNumber, int16_t torqueOffset)
{
	set16bit(oo_torqueOffset, driveNumber, torqueOffset);
}

void EtherCATInterfaceElmo::setTouchProbeFunction(int driveNumber, uint16_t touchProbeFunction)
{
	set16bit(oo_touchProbeFunction, driveNumber, touchProbeFunction);
}

void EtherCATInterfaceElmo::setInterpolatedDataRecord1(int driveNumber, int32_t interpolatedDataRecord1)
{
	set32bit(oo_interpolatedDataRecord_1, driveNumber, interpolatedDataRecord1);
}

void EtherCATInterfaceElmo::setInterpolatedDataRecord2(int driveNumber, int32_t interpolatedDataRecord2)
{
	set32bit(oo_interpolatedDataRecord_2, driveNumber, interpolatedDataRecord2);
}

void EtherCATInterfaceElmo::setTargetVelocity(int driveNumber, int32_t targetVelocity)
{
	set32bit(oo_targetVelocity, driveNumber, targetVelocity);
}

void EtherCATInterfaceElmo::setDigitalOutput(int driveNumber, uint32_t digitalOutput)
{
	set32bit(oo_digitalOutput, driveNumber, digitalOutput);
}

void EtherCATInterfaceElmo::setPolarity(int driveNumber, uint8_t polarity)
{
	set8bit(oo_polarity, driveNumber, polarity);
}

void EtherCATInterfaceElmo::setGainSchedulingManualIndex(int driveNumber, uint16_t index)
{
	set16bit(oo_gainSchedlingManualIndex, driveNumber, index);
}



//TODO casting signed/unsigned?

//basic get functions:
uint16_t EtherCATInterfaceElmo::getStatusWord(int driveNumber)
{
	return get16bit(io_statusWord, driveNumber);
}

int8_t EtherCATInterfaceElmo::getModeOfOperationDisplay(int driveNumber)
{
	return get8bit(io_modeOfOperationDisplay, driveNumber);
}


int32_t EtherCATInterfaceElmo::getPositionDemand_UU(int driveNumber)
{
	return get32bit(io_postionDemand_UU, driveNumber);
}

int32_t EtherCATInterfaceElmo::getActualPosition_counts(int driveNumber)
{
	return get32bit(io_actualPosition_counts, driveNumber);
}

int32_t EtherCATInterfaceElmo::getPositionActualValue(int driveNumber)
{
	return get32bit(io_positionActualValue, driveNumber);
}

int32_t EtherCATInterfaceElmo::getVelocitySensorActualValue(int driveNumber)
{
	return get32bit(io_velocitySensorActualValue, driveNumber);
}

int32_t EtherCATInterfaceElmo::getVelocityDemand(int driveNumber)
{
	return get32bit(io_velocityDemand, driveNumber);
}

int32_t EtherCATInterfaceElmo::getVelocityActualValue(int driveNumber)
{
	return get32bit(io_velocityActualValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::getTorqueDemandValue(int driveNumber)
{
	return get16bit(io_torqueDemandValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::getTorqueActualValue(int driveNumber)
{
	return get16bit(io_torqueActualValue, driveNumber);
}

uint16_t EtherCATInterfaceElmo::getTouchProbeStatus(int driveNumber)
{
	return get16bit(io_touchProbeStatus, driveNumber);
}

int32_t EtherCATInterfaceElmo::getTouchProbePos1Positive(int driveNumber)
{
	return get32bit(io_touchProbePos1Positive, driveNumber);
}

int32_t EtherCATInterfaceElmo::getTouchProbePos1Negative(int driveNumber)
{
	return get32bit(io_touchProbePos1Negative, driveNumber);
}

int32_t EtherCATInterfaceElmo::getTouchProbePos2Positive(int driveNumber)
{
	return get32bit(io_touchProbePos2Positive, driveNumber);
}

uint32_t EtherCATInterfaceElmo::getDCLinkCircuitVoltage(int driveNumber)
{
	return get32bit(io_DCLinkCircuitVoltage, driveNumber);
}

int32_t EtherCATInterfaceElmo::getPositionFollowingError(int driveNumber)
{
	return get32bit(io_positionFollowingError, driveNumber);
}

int32_t EtherCATInterfaceElmo::getControllEffort(int driveNumber)
{
	return get32bit(io_controllEffort, driveNumber);
}

int32_t EtherCATInterfaceElmo::getPositionDemandValue_cnt(int driveNumber)
{
	return get32bit(io_positionDemandValue_cnt, driveNumber);
}

uint32_t EtherCATInterfaceElmo::getDigitalInputs(int driveNumber)
{
	return get32bit(io_digitalInputs, driveNumber);
}

int16_t EtherCATInterfaceElmo::getAnalogInput(int driveNumber)
{
	return get16bit(io_analogInput, driveNumber);
}

int32_t EtherCATInterfaceElmo::getAuxilaryPositionActualValue(int driveNumber)
{
	return get32bit(io_auxilaryPositionActualValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::getCurrentActualValue(int driveNumber)
{
	return get16bit(io_currentActualValue, driveNumber);
}



