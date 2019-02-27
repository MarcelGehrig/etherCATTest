#ifndef ETHERCAT_INTERFACE_BASE_HPP_
#define ETHERCAT_INTERFACE_BASE_HPP_

#include <list>

#include "EtherCATInterfaceBase_config.hpp"
#include <EtherCATMain.hpp>

// nur lokal TODO
// #include "../eeros/installx86/include/EtherCATMain.hpp"


class etherCATInterface::EtherCATInterfaceBase {
public:
	EtherCATInterfaceBase(ecmasterlib::EtherCATMain* etherCATStack);
	
	
	
	// basic set functions:
	void set8bit(int offsetInByte, uint32_t driveNumber, uint8_t payload);
	void set8bit(int offsetInByte, uint32_t driveNumber, int8_t payload);
	void set16bit(int offsetInByte, uint32_t driveNumber, uint16_t payload);
	void set16bit(int offsetInByte, uint32_t driveNumber, int16_t payload);
	void set32bit(int offsetInByte, uint32_t driveNumber, uint32_t payload);
	void set32bit(int offsetInByte, uint32_t driveNumber, int32_t payload);
	
	
	// basic get functions:
	unsigned char get8bit(int offsetInByte, uint32_t driveNumber);
	unsigned short get16bit(int offsetInByte, uint32_t driveNumber);
	unsigned int get32bit(int offsetInByte, uint32_t driveNumber);
	
	
	
// 	//basic set functions:
// 	void setControlWord(int driveNumber, uint16_t word);
// 	void setModeOfOperation(int driveNumber, int8_t mode);
// 	void setTorque(int driveNumber, int16_t torque);
// 	void setPos(int driveNumber, int32_t position);
// 	void setTouchProbeFunction(int driveNumber, uint16_t function);
// 	void setGainSchedulingManualIndex(int driveNumber, uint16_t index);
// 	void setVel(int driveNumber, int32_t velocity);
// 	
// 	void setPosOffset(int driveNumber, int32_t offset);
// 	void setPosAuxOffset(int driveNumber, int32_t offset);
// 	
// 	
// 	//crude debug functions, do not use:
// 	int16_t DBGgetSetTorque();
	bool checkOffset(int offsetInByte);
	bool checkMaskedBits(uint16_t variable, uint16_t compareWord, uint16_t mask);
	void log(std::string message);
	void logError(std::string message);
	
	private:
	ethercat::EtherCATMain* etherCATStack;
	uint8_t* inBuffer;
	uint8_t* outBuffer;
	
// 	touchProbeStateEnum touchProbeState[numberOfDrives];
	
// 		int32_t posAct[numberOfDrives]		= { 0 };
// 		int32_t diff1[numberOfDrives]		= { 0 };
// 		int64_t posNew1[numberOfDrives]		= { 0 };
// 		int32_t posRawOld[numberOfDrives]		= { 0 };
// 		int64_t posOld[numberOfDrives] 		= { 0 };
// 		int32_t posAuxRawOld[numberOfDrives]	= { 0 };
// 		int32_t posAuxOld[numberOfDrives] 		= { 0 };
// 	int32_t posOffset[numberOfDrives]		= { 0 };
// 	int32_t posAuxOffset[numberOfDrives]	= { 0 };
// 	int32_t prevRawPos[numberOfDrives]		= { 0 };
// 	int32_t prevRawAuxPos[numberOfDrives]	= { 0 };
// 	int64_t absPos[numberOfDrives] 		= { 0 };
// 	int64_t absAuxPos[numberOfDrives] 		= { 0 };
	
// 	eeros::logger::Logger log;
	
// 	void checkDriveNumber(int driveNumber);
};





#endif // ETHERCAT_INTERFACE_BASE_HPP_