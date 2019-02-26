#include "EtherCATInterfaceElmo.hpp"

using namespace etherCATInterface;

EtherCATInterfaceElmo::EtherCATInterfaceElmo(ecmasterlib::EtherCATMain* etherCATStack): 
EtherCATInterfaceBase(etherCATStack)
{ }




//advanced functions:
void initElmoDrives()
{
// 	log.trace() << "initElmoDrives()";
	bool allDrivesAreSwitchedOn = false;
	while (!allDrivesAreSwitchedOn) {
		allDrivesAreSwitchedOn = switchOnAllDrives();
	}
	
	for (int drive = 0; drive < numberOfDrives; drive++) {
		setModeOfOperation(drive, driveMode::profileVelocity);
// 		setModeOfOperation(drive, driveMode::profilePosition);
	}
// 	log.trace() << "done";
}

bool recoverAllDrivesFromFault()
{
	return switchOnAllDrives();
}

bool switchOnAllDrives()
{
	bool allDrivesAreSwitchedOn = true;
		for (int driveNumber = 0; driveNumber < numberOfDrives; driveNumber++) {
			auto driveState = getStatus(driveNumber);
			
// 			log.trace() << "drive " << driveNumber << " hase state: 0x" << std::hex << getStatus(driveNumber);
			
			// 0:	'start' -> 'not ready to switch on'					: The drive self-tests and/or self-initializes
			
			// 1:	'not ready to switch on' -> 'switch on disabled'	: The drive activates communication
			
			// 15: 	fault reset
			if (checkMaskedBits( driveState, faultValue, faultMask)) {
				setControlWord(driveNumber, faultReset);
// 				log.trace() << "drive " << driveNumber << " is in 15 fault reset";
			}
			
			
			// 2:	'switch on disabled' -> 'ready to switch on'		:
			if (checkMaskedBits( driveState, switchOnDisabledValue, switchOnDisabledMask)) {
				setControlWord(driveNumber, shutdown);
// 				log.trace() << "drive " << driveNumber << " is in 2: switch on disabled";
			}
			
			// 3:	'ready to switch on' -> 'switched on'				:
			if (checkMaskedBits( driveState, readyToSwitchOnValue, readyToSwitchOnMask)) {
				setControlWord(driveNumber, switchOn);	
// 				log.trace() << "drive " << driveNumber << " is in 3: ready to switch on";
			}
			
			if (!checkMaskedBits( driveState, switchedOnValue, switchedOnMask)) {
				allDrivesAreSwitchedOn = false;
// 				log.trace() << "drive " << driveNumber << " is not ready";
			}
		}
	
	return allDrivesAreSwitchedOn;
}


//advanced get functions:
bool isDriveReady(int driveNumber)
{
//	//DEBUG böse häck
//	if(driveNumber == 0) return true;
	
	
	return checkMaskedBits( getStatus(driveNumber), switchedOnValue, switchedOnMask);
}

bool isAllDrivesReady()
{
	bool ready = true;
	for ( int i; i < numberOfDrives; i++) {
		if ( !isDriveReady(i) ) ready = false;
	}
	return ready;		
}

bool isDriveEnabled(int driveNumber)
{
//	//DEBUG böse häck
//	if(driveNumber == 0) return true;
	
	
	return checkMaskedBits( getStatus(driveNumber), operationEnabledValue, operationEnabledMask);
}

bool isAllDrivesEnabled()
{
	bool ready = true;
	for ( int i; i < numberOfDrives; i++) {
		if ( !isDriveEnabled(i) ) ready = false;
	}
	return ready;
}

int64_t getPos(int driveNumber)
{
	int32_t rawPos = getPosRaw(driveNumber);
	int32_t diff = rawPos - prevRawPos[driveNumber];
	prevRawPos[driveNumber] = rawPos;
	absPos[driveNumber] += static_cast<int64_t>(diff);
 	return absPos[driveNumber] + static_cast<int64_t>(getPosOffset(driveNumber));
}

int64_t getPosAux(int driveNumber)
{
	int32_t rawAuxPos = getPosAuxRaw(driveNumber);
	int32_t diff = rawAuxPos - prevRawAuxPos[driveNumber];
	prevRawAuxPos[driveNumber] = rawAuxPos;
	absAuxPos[driveNumber] += static_cast<int64_t>(diff);;
 	return absAuxPos[driveNumber] + static_cast<int64_t>(getPosAuxOffset(driveNumber));
}



//advanced set functions:
void setControlWord(int driveNumber, movingchair::controlWordCommand word)
{
	switch(word) {
		case controlWordCommand::shutdown :			setControlWord(driveNumber, 0x06);
			break;
		case controlWordCommand::switchOn :			setControlWord(driveNumber, 0x07);
			break;
		case controlWordCommand::disableDrive :		setControlWord(driveNumber, 0x07);	//same as switchOn
			break;
		case controlWordCommand::enableOperation :	setControlWord(driveNumber, 0x0f);
			break;
		case controlWordCommand::faultReset :		setControlWord(driveNumber, 0x80);
			break;
		default :
			break;
	}
	
}

void setModeOfOperation(int driveNumber, movingchair::driveMode mode)
{
	switch(mode) {
		case driveMode::profilePosition :			
			enableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 1); 
			break;
		case driveMode::profileVelocity :		
			enableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 3); 
			break;
		case driveMode::profileTorque :		
			disableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 4); 
			break;
		case driveMode::homing :		
			disableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 6); 
			break;
		case driveMode::interpolatedPosition :
			enableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 7); 
			break;
		case driveMode::cyclicSynchronousPosition :
			enableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 8); 
			break;
		case driveMode::cyclicSynchronousVelocity :
			enableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 9); 
			break;
		case driveMode::cyclicSynchronousTorque :		
			disableVelocityControl(driveNumber);
			setModeOfOperation(driveNumber, 10); 
			break;
		default : 
			break;
	}
}

void disableVelocityControl(int driveNumber)
{
	setGainSchedulingManualIndex(driveNumber, 2);
}

void enableVelocityControl(int driveNumber)
{
	setGainSchedulingManualIndex(driveNumber, 1);
}

void enableDrive(int driveNumber)
{
// 	if ( !checkMaskedBits(getStatus(driveNumber), switchedOnValue, switchedOnMask) ) log.error() << "Drive: " << driveNumber << " needs to be in state 'switched on'";
//	if ( !checkMaskedBits(getStatus(driveNumber), switchedOnValue, switchedOnMask) ) log.error() << "EnableDrive(" << driveNumber << ") with status: 0x" << std::hex << getStatus(driveNumber) << " needs to be in state 'switched on'";
	if ( !checkMaskedBits(getStatus(driveNumber), switchedOnValue, switchedOnMask) and !checkMaskedBits(getStatus(driveNumber), operationEnabledValue, operationEnabledMask) ) {
		log.error() << "EnableDrive(" << driveNumber << ") with status: 0x" << std::hex << getStatus(driveNumber) << " needs to be in state 'switched on'";
	}
	else setControlWord(driveNumber, enableOperation);;
}

void enableAllDrives()
{
	for ( int i; i < numberOfDrives; i++) {
		enableDrive(i);
	}
}

void disableAllDrives()
{
	for ( int i; i < numberOfDrives; i++) {
		disable(i);
	}
}

void disable(int driveNumber)
{
	setControlWord(driveNumber, disableDrive);
}


//index pulse
// void homeWithIndexPulse(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > offsets, bool auxPos)
// {
// 	std::array< int, numberOfWheels > touchProbes;
// // 	touchProbes.resize( driveNumbers.size() );
// 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// 	return homeWithIndexPulse(driveNumbers, offsets, touchProbes);
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

bool enableCapturingIndexPulse(std::vector< int > driveNumbers )
{
	std::vector< int > touchProbes;
	touchProbes.resize( driveNumbers.size() );
	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
	return enableCapturingIndexPulse(driveNumbers, touchProbes);

}

bool enableCapturingIndexPulse(std::vector< int > driveNumbers, std::vector< int > touchProbes)
{
	if ( (driveNumbers.size()!=touchProbes.size()) ) {
		log.error() << "enableCapturingIndexPulse(...) all argument vectors have to be of the same size";
		return false;
	}
	
	bool allProbesReady = true;
	for (	std::vector< int >::iterator itDriveNumbers=driveNumbers.begin(),
			itTouchProbes=touchProbes.begin();
			itDriveNumbers != driveNumbers.end();
			++itDriveNumbers, ++itTouchProbes)
	{
			if ( !enableCapturingIndexPulse(*itDriveNumbers, *itTouchProbes) )	allProbesReady = false;
	}
	
	return allProbesReady;
}

bool enableCapturingIndexPulse(int driveNumber, int touchProbe)
{
	if ( touchProbe != 1 ) {
		log.error() << "touchProbe " << touchProbe << "is not implemented";
		return false;
	}
	
	
	switch(touchProbeState[driveNumber]) {
		case touchProbeStateEnum::reset :
			setTouchProbeFunction( driveNumber, 0x0000 );		// disable everything
			if ( getTouchProbeStatus(driveNumber) == 0x0000 )	// touch probe is swithced off  -> stored values on elmo are reset
				touchProbeState[driveNumber] = touchProbeStateEnum::enableProbe;
			return false;
			break;
		case touchProbeStateEnum::enableProbe :
			setTouchProbeFunction( driveNumber, 0x0001 );		//0x01 enable touchProbe1; disabled sampling -> stored value on elmo is reset
			if ( getTouchProbeStatus(driveNumber) == 0x0001 )	// touch probe is enabled again
				touchProbeState[driveNumber] = touchProbeStateEnum::enableSampling;
			return false;
			break;
		case touchProbeStateEnum::enableSampling :
			setTouchProbeFunction( driveNumber, 0x0031 );		//0x31 enable sampling on positive and negative edge
			touchProbeState[driveNumber] = touchProbeStateEnum::enabled;
			return true;
			break;
		case touchProbeStateEnum::enabled :
			return true;
			break;
		default :
			log.error() << "touchProbeStateEnum does not exist";
	}
}


bool disableCapturingIndexPulse(int driveNumber, int touchProbe)
{
	if ( touchProbe != 1 ) {
		log.error() << "touchProbe " << touchProbe << "is not implemented";
		return false;
	}
	
	setTouchProbeFunction( driveNumber, 0x0000 );		// disable everything
	if ( getTouchProbeStatus(driveNumber) == 0x0000 ) {	// touch probe is swithced off  -> stored values on elmo are reset 
		touchProbeState[driveNumber] = touchProbeStateEnum::reset;
		return true;
	}
	return false;
}


// void waitForAllIndexPulses(std::array< int, numberOfWheels > driveNumbers, int pollingTimeUSec)
// {
// 	std::array< int, numberOfWheels > touchProbes;
// // 	touchProbes.resize( driveNumbers.size() );
// 	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
// 	return waitForAllIndexPulses(driveNumbers, touchProbes);
// 
// }
// 
// void waitForAllIndexPulses(std::array< int, numberOfWheels > driveNumbers, std::array< int, numberOfWheels > touchProbes, int pollingTimeUSec)
// {	
// 	bool allPulsesCaptured = false;
// 	while ( !allPulsesCaptured ) {
// 			for (	std::array< int, numberOfWheels >::iterator itDriveNumbers=driveNumbers.begin(),
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
// void waitForAllIndexPulses(int driveNumber, int touchProbe, int pollingTimeUSec)
// {
// 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) usleep(pollingTimeUSec);
// }


void setOffsetAtIndexPos(std::vector< int > driveNumbers, bool isAuxPos, std::vector< int > offsets)
{
	std::vector< int > touchProbes;
	touchProbes.resize( driveNumbers.size() );
	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
	return setOffsetAtIndexPos(driveNumbers, isAuxPos, offsets, touchProbes);

}

void setOffsetAtIndexPos(std::vector< int > driveNumbers, bool isAuxPos, std::vector< int > offsets, std::vector< int > touchProbes)
{
	for (	std::vector< int >::iterator itDriveNumbers=driveNumbers.begin(),
			itOffsets=offsets.begin(),
			itTouchProbes=touchProbes.begin();
			itDriveNumbers != driveNumbers.end();
			++itDriveNumbers, ++itOffsets, ++itTouchProbes) {
				setOffsetAtIndexPos(*itDriveNumbers, isAuxPos, *itOffsets, *itTouchProbes);
	}

}

void setOffsetAtIndexPos(int driveNumber, bool isAuxPos, int offset, int touchProbe)
{
	log.trace() << "Argument offset: " << offset;
	log.trace() << "captured Position: " << getCapturedPosition(driveNumber, touchProbe);
	int32_t offs = static_cast<int32_t>( - static_cast<int>( getCapturedPosition(driveNumber, touchProbe) ) + offset );
	log.trace() << "setOffsetAtIndexPos(...) offs: " << offs;
	isAuxPos ? setPosAuxOffset(driveNumber, offs) : setPosOffset(driveNumber, offs);
}

bool getIndexPulseIsCaptured(int driveNumber, int touchProbe)
{
	if ( getIndexPulsePositiveEdgeIsCaptured(driveNumber, touchProbe) || getIndexPulseNegativeEdgeIsCaptured(driveNumber, touchProbe) ) {
		return true;
	}
	
	return false;
}

bool getIndexPulsePositiveEdgeIsCaptured(int driveNumber, int touchProbe)
{	
	uint16_t touchProbeStatus = getTouchProbeStatus(driveNumber);
	// 0x0060 both; 0x0040 negative edge; 0x0020 positive edge
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, 0x0002, 0x0002) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, 0x0200, 0x0200) ) return true;
	}
	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
	return false;
}

bool getIndexPulseNegativeEdgeIsCaptured(int driveNumber, int touchProbe)
{
	uint16_t touchProbeStatus = getTouchProbeStatus(driveNumber);
	// 0x0060 both; 0x0040 negative edge; 0x0020 positive edge
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, 0x0004, 0x0004) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, 0x0400, 0x0400) ) return true;
	}
	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
	return false;
	
}

bool getIndexPulseIsCapturedIsValid(int driveNumber, int touchProbe)
{
	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) {
		log.warn() << "isDirectionOfRotationPositive(...) index pulse is not captured";
		return false;
	}
	
	if ( touchProbe == 1 ) {
		auto positiveEdgeValue = getCapturedPositionPositivePulse(driveNumber, touchProbe);
		auto negativeEdgeValue = getCapturedPositionNegativePulse(driveNumber, touchProbe);
		
		//check if both physical index pulses are captured
		if ( abs( abs(positiveEdgeValue) - abs(negativeEdgeValue) ) < 32000 ) {	// 0/1
			log.trace() << "Positve edge value ( " << positiveEdgeValue << " ) is to similar to negative edge value ( " << negativeEdgeValue 
			<< " ). Difference: " << abs( abs(positiveEdgeValue) - abs(negativeEdgeValue) );
			return false;
		}
		else {
			return true;
		}
	}
	else {
		log.error() << "Touch probe " << touchProbe << " is not implemented.";
		return false;
	}
}

int32_t getCapturedPosition(int driveNumber, int touchProbe)	// compensated for direction of rotation
{	

	return getCapturedPositionPositivePulse(driveNumber, touchProbe);
  
// 	// Bastel aus Zeiten deffekter Magnetringe
// 	if ( !getIndexPulseIsCaptured(driveNumber, touchProbe) ) log.warn() << "isDirectionOfRotationPositive(...) index pulse is not captured";
// 	
// 	auto positiveEdgeValue = getCapturedPositionPositivePulse(driveNumber, touchProbe);
// 	auto negativeEdgeValue = getCapturedPositionNegativePulse(driveNumber, touchProbe);
// 
// 	if (abs( abs(positiveEdgeValue) - abs(negativeEdgeValue)) < 200000) {
// 		if (positiveEdgeValue < negativeEdgeValue  ) // positive
// 			return getCapturedPositionPositivePulse(driveNumber, touchProbe);
// 		else // negative
// 			return getCapturedPositionNegativePulse(driveNumber, touchProbe);
// 	} 
// 	else {
// 		if (positiveEdgeValue < negativeEdgeValue  ) // positive
// 			return getCapturedPositionNegativePulse(driveNumber, touchProbe);
// 		else // negative
// 			return getCapturedPositionPositivePulse(driveNumber, touchProbe);
// 	}
}

int32_t getCapturedPositionPositivePulse(int driveNumber, int touchProbe)
{
	int32_t position;
	if	( touchProbe == 1 ) position = getTouchProbePos1Positive(driveNumber);
	else if	( touchProbe == 2 ) position = getTouchProbePos2PosValue(driveNumber);
	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
	return position;
}

int32_t getCapturedPositionNegativePulse(int driveNumber, int touchProbe)
{
	int32_t position;
	if	( touchProbe == 1 ) position = getTouchProbePos1Negative(driveNumber);
	else if	( touchProbe == 2 ) log.error() << "No TouchProbePosition negative pulse for TouchProbe 2.";
	else	log.error() << "touchProbe " << touchProbe << " does not exists.";
	return position;
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
void EtherCATInterfaceElmo::setControlWord(int driveNumber, uint16_t controlWord)
{
	EtherCATInterfaceBase::set16bit(oo_controlWord, driveNumber, controlWord);
}

void EtherCATInterfaceElmo::setModeOfOperation(int driveNumber, int8_t modeOfOperation)
{

}

void EtherCATInterfaceElmo::setTargetTorque(int driveNumber, int16_t targetTorque)
{

}

void EtherCATInterfaceElmo::setMaxCurrent(int driveNumber, int16_t maxCurrent)
{

}

void EtherCATInterfaceElmo::setTargetPosition(int driveNumber, int32_t targetPosition)
{

}

void EtherCATInterfaceElmo::setMaxProfileVelocity(int driveNumber, uint32_t maxProfileVelocity)
{

}

void EtherCATInterfaceElmo::setProfileVelocity(int driveNumber, uint32_t profileVelocity)
{

}

void EtherCATInterfaceElmo::setEndVelocity(int driveNumber, uint32_t endVelocity)
{

}

void EtherCATInterfaceElmo::setProfileAcceleration(int driveNumber, uint32_t profileAcceleration)
{

}

void EtherCATInterfaceElmo::setProfileDeceleration(int driveNumber, uint32_t ProfileDeceleration)
{

}

void EtherCATInterfaceElmo::setTorqueSlope(int driveNumber, uint32_t torqueSlope)
{

}

int32_t EtherCATInterfaceElmo::getActualPosition_counts(int driveNumber)
{

}

void EtherCATInterfaceElmo::setVelocityOffset(int driveNumber, int32_t velocityOffset)
{

}

void EtherCATInterfaceElmo::setTorqueOffset(int driveNumber, int16_t torqueOffset)
{

}

int32_t EtherCATInterfaceElmo::getTouchProbePos1Negative(int driveNumber)
{

}

void EtherCATInterfaceElmo::setInterpolatedDataRecord1(int driveNumber, int32_t interpolatedDataRecord1)
{

}

void EtherCATInterfaceElmo::setInterpolatedDataRecord2(int driveNumber, int32_t interpolatedDataRecord2)
{

}

void EtherCATInterfaceElmo::setTargetVelocity(int driveNumber, int32_t targetVelocity)
{

}

void EtherCATInterfaceElmo::setDigitalOutput(int driveNumber, uint32_t digitalOutput)
{

}

void EtherCATInterfaceElmo::setPolarity(int driveNumber, uint8_t polarity)
{

}


//basic get functions:
uint16_t EtherCATInterfaceElmo::getStatusWord(int driveNumber)
{
	return EtherCATInterfaceBase::get16bit(io_statusWord, driveNumber);
}

uint8_t EtherCATInterfaceElmo::getModeOfOperationDisplay(int driveNumber)
{

}


int32_t EtherCATInterfaceElmo::getPositionDemand_UU(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::ActualPosition_counts(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getPositionActualValue(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getVelocitySensorActualValue(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getVelocityDemand(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getVelocityActualValue(int driveNumber)
{

}

int16_t EtherCATInterfaceElmo::getTorqueDemandValue(int driveNumber)
{

}

int16_t EtherCATInterfaceElmo::getTorqueActualValue(int driveNumber)
{

}

uint16_t EtherCATInterfaceElmo::getTouchProbeStatus(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getTouchProbePos1Positive(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getTouchProbePos1Negative(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getTouchProbePos2Positive(int driveNumber)
{

}

uint32_t EtherCATInterfaceElmo::getDCLinkCircuitVoltage(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getPositionFollowingError(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getControllEffort(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getPositionDemandValue_cnt(int driveNumber)
{

}

uint32_t EtherCATInterfaceElmo::getDigitalInputs(int driveNumber)
{

}

int16_t EtherCATInterfaceElmo::getAnalogInput(int driveNumber)
{

}

int32_t EtherCATInterfaceElmo::getAuxilaryPositionActualValue(int driveNumber)
{

}

int16_t EtherCATInterfaceElmo::getCurrentActualValue(int driveNumber)
{

}


	
	

/*
// // Pseudolock
// // //////////////////
// basic get functions:
uint16_t getStatus(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmWord( inBuffer + driveNumber*byteSizePerSlave + 0 );
}

int8_t getModeOfOperation(int driveNumber)
{
//	checkDriveNumber( driveNumber );
 	return *( inBuffer + driveNumber*byteSizePerSlave + 2 );
}

int32_t getPosRaw(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	int32_t pos = etherCATStack->getFrmDWord( inBuffer + driveNumber*byteSizePerSlave + 4 );
	return pos;
}

int32_t getVel(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmDWord( inBuffer + driveNumber*byteSizePerSlave + 8 );

}

int16_t getTorque(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmWord( inBuffer + driveNumber*byteSizePerSlave + 12 );
}

uint16_t getTouchProbeStatus(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmWord( inBuffer + driveNumber*byteSizePerSlave + 14 );
}

int32_t getTouchProbePos1Positive(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmDWord( inBuffer + driveNumber*byteSizePerSlave + 16 );
}

int32_t getTouchProbePos1Negative(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmDWord( inBuffer + driveNumber*byteSizePerSlave + 20 );
}

int32_t getTouchProbePos2PosValue(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return etherCATStack->getFrmDWord( inBuffer + driveNumber*byteSizePerSlave + 24 );
}

int32_t getPosAuxRaw(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	int32_t pos = etherCATStack->getFrmDWord( inBuffer + driveNumber*byteSizePerSlave + 28 );
	return pos;
}

int32_t getPosOffset(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return posOffset[driveNumber];
}

int32_t getPosAuxOffset(int driveNumber)
{
//	checkDriveNumber( driveNumber );
	return posAuxOffset[driveNumber];
}



// basic set funcitons:
void setControlWord(int driveNumber, uint16_t word)
{
//	checkDriveNumber( driveNumber );
	etherCATStack->setWord(outBuffer + driveNumber*byteSizePerSlave + 0, word);
}

void setModeOfOperation(int driveNumber, int8_t mode)
{
//	checkDriveNumber( driveNumber );
	etherCATStack->setByte(outBuffer + driveNumber*byteSizePerSlave + 2, mode);
}

void setTorque(int driveNumber, int16_t torque)
{
//	checkDriveNumber( driveNumber );
// 	if ( torque >= 0 AND torque <= 0 ) {
		etherCATStack->setDWord(outBuffer + driveNumber*byteSizePerSlave + 4, torque);
// 	}
}

void setPos(int driveNumber, int32_t position)
{
//	checkDriveNumber( driveNumber );
	etherCATStack->setDWord(outBuffer + driveNumber*byteSizePerSlave + 6, position);
}

void setTouchProbeFunction(int driveNumber, uint16_t function)
{
//	checkDriveNumber( driveNumber );
	etherCATStack->setWord(outBuffer + driveNumber*byteSizePerSlave + 10, function);
}

void setGainSchedulingManualIndex(int driveNumber, uint16_t index)
{
//	checkDriveNumber( driveNumber );
	etherCATStack->setWord(outBuffer + driveNumber*byteSizePerSlave + 12, index);
}

void setVel(int driveNumber, int32_t velocity)
{
//	checkDriveNumber( driveNumber );
// 	etherCATStack->setDWord(outBuffer + driveNumber*byteSizePerSlave + 14, velocity);
	EtherCATInterfaceBase::set32bit(14, driveNumber, velocity);
}


void setPosOffset(int driveNumber, int32_t offset)
{
//	checkDriveNumber( driveNumber );
	posOffset[driveNumber] = offset;
}
void setPosAuxOffset(int driveNumber, int32_t offset)
{
//	checkDriveNumber( driveNumber );
	posAuxOffset[driveNumber] = offset;
}



*/

// // original
// // ///////////////
// // basic get functions:
// uint16_t getStatus(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_WORD( inBuffer + driveNumber*byteSizePerSlave + 0 );
// }
// 
// 
// int32_t getPosRaw(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	int32_t pos = EC_GET_FRM_DWORD( inBuffer + driveNumber*byteSizePerSlave + 2 );
// 	return pos;
// }
// 
// int32_t getVel(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_DWORD( inBuffer + driveNumber*byteSizePerSlave + 6 );
// 
// }
// 
// int16_t getTorque(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_WORD( inBuffer + driveNumber*byteSizePerSlave + 10 );
// }
// 
// uint16_t getTouchProbeStatus(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_WORD( inBuffer + driveNumber*byteSizePerSlave + 12 );
// }
// 
// int32_t getTouchProbePos1Positive(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_DWORD( inBuffer + driveNumber*byteSizePerSlave + 14 );
// }
// 
// int32_t getTouchProbePos1Negative(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_DWORD( inBuffer + driveNumber*byteSizePerSlave + 18 );
// }
// 
// int32_t getTouchProbePos2PosValue(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return EC_GET_FRM_DWORD( inBuffer + driveNumber*byteSizePerSlave + 22 );
// }
// 
// int32_t getPosAuxRaw(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	int32_t pos = EC_GET_FRM_DWORD( inBuffer + driveNumber*byteSizePerSlave + 26 );
// 	return pos;
// }
// 
// int32_t getPosOffset(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return posOffset[driveNumber];
// }
// 
// int32_t getPosAuxOffset(int driveNumber)
// {
// //	checkDriveNumber( driveNumber );
// 	return posAuxOffset[driveNumber];
// }
// 
// 
// 
// // basic set funcitons:
// void setControlWord(int driveNumber, uint16_t word)
// {
// //	checkDriveNumber( driveNumber );
// 	EC_SETWORD(outBuffer + driveNumber*byteSizePerSlave + 0, word);
// }
// 
// void setModeOfOperation(int driveNumber, int8_t mode)
// {
// //	checkDriveNumber( driveNumber );
// 	std::cout << "outBuffer: " << driveNumber*byteSizePerSlave + 2 << "     mode: " << mode;
// 	*(outBuffer + driveNumber*byteSizePerSlave + 2) = mode;
// }
// 
// void setTorque(int driveNumber, int16_t torque)
// {
// //	checkDriveNumber( driveNumber );
// 	EC_SETDWORD(outBuffer + driveNumber*byteSizePerSlave + 4, torque);
// }
// 
// void setPos(int driveNumber, int32_t position)
// {
// //	checkDriveNumber( driveNumber );
// 	EC_SETDWORD(outBuffer + driveNumber*byteSizePerSlave + 6, position);
// }
// 
// void setTouchProbeFunction(int driveNumber, uint16_t function)
// {
// //	checkDriveNumber( driveNumber );
// 	EC_SETWORD(outBuffer + driveNumber*byteSizePerSlave + 10, function);
// }
// 
// void setVel(int driveNumber, int32_t velocity)
// {
// //	checkDriveNumber( driveNumber );
// 	EC_SETDWORD(outBuffer + driveNumber*byteSizePerSlave + 12, velocity);
// }
// 
// 
// void setPosOffset(int driveNumber, int32_t offset)
// {
// //	checkDriveNumber( driveNumber );
// 	posOffset[driveNumber] = offset;
// }
// void setPosAuxOffset(int driveNumber, int32_t offset)
// {
// //	checkDriveNumber( driveNumber );
// 	posAuxOffset[driveNumber] = offset;
// }
// 
// 
// 
// //private:
// bool checkMaskedBits(uint16_t variable, uint16_t compareWord, uint16_t mask)
// {
// 	variable = variable & mask;
// 	compareWord = compareWord & mask;
// 	if ( variable == compareWord ) return true;
// 	else return false;
// }
// 
// void checkDriveNumber(int driveNumber)
// {
// 	if ( driveNumber > numberOfDrives ) log.error() << "driveNumber is out of range: " << driveNumber;
// }


