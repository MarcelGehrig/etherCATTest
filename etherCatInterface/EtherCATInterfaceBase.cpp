#include "EtherCATInterfaceBase.hpp"

using namespace etherCATInterface;

EtherCATInterfaceBase::EtherCATInterfaceBase(ecmasterlib::EtherCATMain* etherCATStack) :
etherCATStack(etherCATStack)
{
	outBuffer = etherCATStack->getOutBuffer();
	inBuffer = etherCATStack->getInBuffer();
	memset(outBuffer, 0, bytesPerPDOFrameTx);
	
// 	for (unsigned int i=0; i<sizeof(touchProbeState); i++) {
// 		touchProbeState[i] = touchProbeStateEnum::reset;
// 	}
}

void EtherCATInterfaceBase::set8bit(uint32_t offsetInByte, uint32_t driveNumber, int8_t payload)
{	
	if(checkOffset(offsetInByte)) {
		etherCATStack->setByte(outBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte, payload);
	}
}

void EtherCATInterfaceBase::set8bit(uint32_t offsetInByte, uint32_t driveNumber, uint8_t payload)
{
	if(checkOffset(offsetInByte)) {
		etherCATStack->setByte(outBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte, payload);
	}
}

void EtherCATInterfaceBase::set16bit(uint32_t offsetInByte, uint32_t driveNumber, int16_t payload)
{
	if(checkOffset(offsetInByte)) {
		etherCATStack->setWord(outBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte, payload);
	}
}

void EtherCATInterfaceBase::set16bit(uint32_t offsetInByte, uint32_t driveNumber, uint16_t payload)
{
	if(checkOffset(offsetInByte)) {
		etherCATStack->setWord(outBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte, payload);
	}
}

void EtherCATInterfaceBase::set32bit(uint32_t offsetInByte, uint32_t driveNumber, int32_t payload)
{
	if(checkOffset(offsetInByte)) {
		etherCATStack->setDWord(outBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte, payload);
	}
}

void EtherCATInterfaceBase::set32bit(uint32_t offsetInByte, uint32_t driveNumber, uint32_t payload)
{
	if(checkOffset(offsetInByte)) {
		etherCATStack->setDWord(outBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte, payload);
	}
}



uint8_t EtherCATInterfaceBase::get8bit(uint32_t offsetInByte, uint32_t driveNumber)
{
	if(checkOffset(offsetInByte)) {
		return (uint8_t)etherCATStack->getFrmByte(inBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte);
	}
}

uint16_t EtherCATInterfaceBase::get16bit(uint32_t offsetInByte, uint32_t driveNumber)
{
// 	return etherCATStack->getFrmWord(inBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte);
//  	std::cout << "IBase: get16bit: driveNumber: " << driveNumber << "   offsetInByte: " << std::dec << driveNumber*bytesPerPDOFrameTx + offsetInByte << std::endl;
// 	auto data16 = etherCATStack->getFrmWord(inBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte);
//  	std::cout << "IBase: get16bit: driveNumber: " << driveNumber << "   offsetInByte: " << offsetInByte << "   data: 0x" << std::hex << data16 << std::endl;
// 	return data16;
	
	if(checkOffset(offsetInByte)) {
// 		std::cout << "IBase: Offset ok" << std::endl;
		return (uint16_t)etherCATStack->getFrmWord(inBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte);
	}
}

uint32_t EtherCATInterfaceBase::get32bit(uint32_t offsetInByte, uint32_t driveNumber)
{
	if(checkOffset(offsetInByte)) {
		return (uint32_t)etherCATStack->getFrmDWord(inBuffer + driveNumber*bytesPerPDOFrameTx + offsetInByte);
	}
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
	etherCATStack->setDWord(outBuffer + driveNumber*byteSizePerSlave + 14, velocity);
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
}*/



//private:
bool EtherCATInterfaceBase::checkOffset(int offsetInByte) {
	if(offsetInByte < 0) {
		std::cout << "ERROR: the PDO offset is not defined in EtherCATInterface. Offset: " << offsetInByte << std::endl; //TODO log
		return false;
	}
	else {
		return true;
	}
}

bool EtherCATInterfaceBase::checkMaskedBits(uint16_t variable, uint16_t compareWord, uint16_t mask)
{
	variable = variable & mask;
	compareWord = compareWord & mask;
	if ( variable == compareWord ) return true;
	else return false;
}

void EtherCATInterfaceBase::log(std::__cxx11::string message)
{
	std::cout << message << std::endl;
}

void EtherCATInterfaceBase::logError(std::__cxx11::string message)
{
	log(message);
}

// void checkDriveNumber(int driveNumber)
// {
// // 	if ( driveNumber > numberOfDrives ) log.error() << "driveNumber is out of range: " << driveNumber;
// }




