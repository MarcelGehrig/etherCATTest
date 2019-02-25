#ifndef ETHERCAT_INTERFACE_HPP_
#define ETHERCAT_INTERFACE_HPP_

#include <list>

#include "EtherCatInterface_config.hpp"



class EtherCatInterface {
public:
	EtherCatInterface(EtherCATMain* etherCATStack);
	
	//advanced functions
	void initAllDrives();
	bool recoverAllDrivesFromFault();
	bool switchOnAllDrives();
	
	//advanced get functions
	MovingChairEtherCAT* getEtherCATStack() { return etherCATStack; };
	bool isDriveReady(int driveNumber);
	bool isAllDrivesReady();
	bool isDriveEnabled(int driveNumber);
	bool isAllDrivesEnabled();
	int64_t getPos(int driveNumber);
	int64_t getPosAux(int driveNumber);
	
	//advanced set functions:
	void setControlWord(int driveNumber, controlWordCommand word);
	void setModeOfOperation(int driveNumber, driveMode mode);
	void disableVelocityControl(int driveNumber);
	void enableVelocityControl(int driveNumber);
	void enableDrive(int driveNumber);
	void enableAllDrives();
	void disable(int driveNumber);
	void disableAllDrives();
	
	//index pulse
	//	at the moment, only one touchProbe at a time can be used
// 		void homeWithIndexPulse(std::array<int, numberOfWheels> driveNumbers, std::array<int, numberOfWheels> offsets, std::array<bool, numberOfWheels> auxPos);	//parallel multiple drives
// 		void homeWithIndexPulse(std::array<int, numberOfWheels> driveNumbers, std::array<int, numberOfWheels> offsets, std::array<bool, numberOfWheels> auxPos, std::array<int, numberOfWheels> touchProbes);	//
// 		int32_t homeWithIndexPulse(int driveNumber, int offset=0, bool auxPos, int touchProbe=1);
	bool enableCapturingIndexPulse(std::vector<int> driveNumbers);
	bool enableCapturingIndexPulse(std::vector<int> driveNumbers, std::vector<int> touchProbes);
	bool enableCapturingIndexPulse(int driveNumber, int touchProbe=1);
	bool disableCapturingIndexPulse(int driveNumber, int touchProbe=1);
// 		void waitForAllIndexPulses(std::array<int, numberOfWheels> driveNumbers, int pollingTimeUSec=1e5);
// 		void waitForAllIndexPulses(std::array<int, numberOfWheels> driveNumbers, std::array<int, numberOfWheels> touchProbes, int pollingTimeUSec=1e5);
// 		void waitForAllIndexPulses(int driveNumber, int touchProbe=1, int pollingTimeUSec=1e5);
	void setOffsetAtIndexPos(std::vector<int> driveNumbers, bool isAuxPos, std::vector<int> offsets);
	void setOffsetAtIndexPos(std::vector<int> driveNumbers, bool isAuxPos, std::vector<int> offsets, std::vector<int> touchProbes);
	void setOffsetAtIndexPos(int driveNumber, bool isAuxPos, int offset=0, int touchProbe=1);
	
	bool getIndexPulseIsCaptured(int driveNumber, int touchProbe=1);	// both pulses are captured
	bool getIndexPulsePositiveEdgeIsCaptured(int driveNumber, int touchProbe=1);
	bool getIndexPulseNegativeEdgeIsCaptured(int driveNumber, int touchProbe=1);
	bool getIndexPulseIsCapturedIsValid(int driveNumber, int touchProbe=1);
	int32_t getCapturedPosition(int driveNumber, int touchProbe=1);			// compensated for direction of rotation
	int32_t getCapturedPositionPositivePulse(int driveNumber, int touchProbe=1);
	int32_t getCapturedPositionNegativePulse(int driveNumber, int touchProbe=1);
// 		bool isDirectionOfRotationPositive(int driveNumber, int touchProbe=1);
	
	//basic get functions:
	uint16_t getStatus(int driveNumber);	//driveNumber is counted from 0
	int8_t getModeOfOperation(int driveNumber);	// 1=position 3=velocity 4=torque
	int32_t getPosRaw(int driveNumber);
	int32_t getVel(int driveNumber);
	int16_t getTorque(int driveNumber);
	uint16_t getTouchProbeStatus(int driveNumber);
	int32_t getTouchProbePos1Positive(int driveNumber);
	int32_t getTouchProbePos1Negative(int driveNumber);
	int32_t getTouchProbePos2PosValue(int driveNumber);
	int32_t getPosAuxRaw(int driveNumber);
	
	int32_t getPosOffset(int driveNumber);
	int32_t getPosAuxOffset(int driveNumber);
	
	
	//basic set functions:
	void setControlWord(int driveNumber, uint16_t word);
	void setModeOfOperation(int driveNumber, int8_t mode);
	void setTorque(int driveNumber, int16_t torque);
	void setPos(int driveNumber, int32_t position);
	void setTouchProbeFunction(int driveNumber, uint16_t function);
	void setGainSchedulingManualIndex(int driveNumber, uint16_t index);
	void setVel(int driveNumber, int32_t velocity);
	
	void setPosOffset(int driveNumber, int32_t offset);
	void setPosAuxOffset(int driveNumber, int32_t offset);
	
	
	//crude debug functions, do not use:
	int16_t DBGgetSetTorque();
	
	
// 	private:
	ethercat::EtherCATMain* etherCATStack;
	uint8_t* inBuffer;
	uint8_t* outBuffer;
	
	touchProbeStateEnum touchProbeState[numberOfDrivesTotal];
	
// 		int32_t posAct[numberOfDrivesTotal]		= { 0 };
// 		int32_t diff1[numberOfDrivesTotal]		= { 0 };
// 		int64_t posNew1[numberOfDrivesTotal]		= { 0 };
// 		int32_t posRawOld[numberOfDrivesTotal]		= { 0 };
// 		int64_t posOld[numberOfDrivesTotal] 		= { 0 };
// 		int32_t posAuxRawOld[numberOfDrivesTotal]	= { 0 };
// 		int32_t posAuxOld[numberOfDrivesTotal] 		= { 0 };
	int32_t posOffset[numberOfDrivesTotal]		= { 0 };
	int32_t posAuxOffset[numberOfDrivesTotal]	= { 0 };
	int32_t prevRawPos[numberOfDrivesTotal]		= { 0 };
	int32_t prevRawAuxPos[numberOfDrivesTotal]	= { 0 };
	int64_t absPos[numberOfDrivesTotal] 		= { 0 };
	int64_t absAuxPos[numberOfDrivesTotal] 		= { 0 };
	
	eeros::logger::Logger log;
	
	bool checkBits(uint16_t variable, uint16_t compareWord, uint16_t mask);
	void checkDriveNumber(int driveNumber);
};








#endif // ETHERCAT_INTERFACE_HPP_