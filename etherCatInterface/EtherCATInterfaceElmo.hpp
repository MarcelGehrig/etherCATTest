#ifndef ETHERCAT_INTERFACE_ELMO_HPP_
#define ETHERCAT_INTERFACE_ELMO_HPP_

#include <list>

#include "EtherCATInterfaceBase.hpp"
#include "EtherCATInterfaceElmo_config.hpp"



class etherCATInterface::EtherCATInterfaceElmo : public etherCATInterface::EtherCATInterfaceBase {
public:
	EtherCATInterfaceElmo(ecmasterlib::EtherCATMain* etherCATStack);
	
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
// 	void setControlWord(int driveNumber, controlWordCommand word);
	void disableVelocityControl(int driveNumber);
	void enableVelocityControl(int driveNumber);
	void enableDrive(int driveNumber);
	void disableAllDrives();
	
	// gain scheduling functions
	void disableVelocityControl(int driveNumber);
	void enableVelocityControl(int driveNumber);
	
	
	

	
	// basic functions
	void disableDrive(int driveNumber);
	void enableDrive();
	void setModeOfOperation(int driveNumber, driveModeOfOperation_ELMO mode, bool scheduleGainIndex = false);
	
	bool getIsDriveEnabled(int driveNumber);
	driveModeOfOperation_ELMO getDriveModeElmo(int driveNumber);
	bool checkDriveStatus(int driveNumber, driveStatus_ELMO driveStatusToCheck);
	driveStatus_ELMO getDriveStatusElmo(int driveNumber);
	
	
	
	
	
	
	// Low level get and set functions to get/write values from/to buffer array
	// ////////////////////////////////////////////////////////////////////
	
	// low level set functions:
	void setControlWord(int driveNumber, uint16_t controlWord);
	void setModeOfOperation(int driveNumber, int8_t modeOfOperation);
	void setTargetTorque(int driveNumber, int16_t targetTorque);
	void setMaxTorque(int driveNumber, int16_t maxTorque);
	void setMaxCurrent(int driveNumber, int16_t maxCurrent);
	void setTargetPosition(int driveNumber, int32_t targetPosition);
	void setMaxProfileVelocity(int driveNumber, uint32_t maxProfileVelocity);
	void setProfileVelocity(int driveNumber, uint32_t profileVelocity);
	void setEndVelocity(int driveNumber, uint32_t endVelocity);
	void setProfileAcceleration(int driveNumber, uint32_t profileAcceleration);
	void setProfileDeceleration(int driveNumber, uint32_t profileDeceleration);
	void setTorqueSlope(int driveNumber, uint32_t torqueSlope);
	void setPositionOffset(int driveNumber, int32_t positionOffset);
	void setVelocityOffset(int driveNumber, int32_t velocityOffset);
	void setTorqueOffset(int driveNumber, int16_t torqueOffset);
	void setTouchProbeFunction(int driveNumber, uint16_t touchProbeFunction);
	void setInterpolatedDataRecord1(int driveNumber, int32_t interpolatedDataRecord1);
	void setInterpolatedDataRecord2(int driveNumber, int32_t interpolatedDataRecord2);
	void setTargetVelocity(int driveNumber, int32_t targetVelocity);
	void setDigitalOutput(int driveNumber, uint32_t digitalOutput);
	void setPolarity(int driveNumber, uint8_t polarity);
	
	void setGainSchedulingManualIndex(int driveNumber, uint16_t index);
	
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
	
	
	
	// low level get functions:
	uint16_t getStatusWord(int driveNumber);
	int8_t getModeOfOperationDisplay(int driveNumber);
	int32_t getPositionDemand_UU(int driveNumber);
	int32_t getActualPosition_counts(int driveNumber);
	int32_t getPositionActualValue(int driveNumber);
	int32_t getVelocitySensorActualValue(int driveNumber);
	int32_t getVelocityDemand(int driveNumber);
	int32_t getVelocityActualValue(int driveNumber);
	int16_t getTorqueDemandValue(int driveNumber);
	int16_t getTorqueActualValue(int driveNumber);
	uint16_t getTouchProbeStatus(int driveNumber);
	int32_t getTouchProbePos1Positive(int driveNumber);
	int32_t getTouchProbePos1Negative(int driveNumber);
	int32_t getTouchProbePos2Positive(int driveNumber);
	uint32_t getDCLinkCircuitVoltage(int driveNumber);
	int32_t getPositionFollowingError(int driveNumber);
	int32_t getControllEffort(int driveNumber);
	int32_t getPositionDemandValue_cnt(int driveNumber);
	uint32_t getDigitalInputs(int driveNumber);
	int16_t getAnalogInput(int driveNumber);
	int32_t getAuxilaryPositionActualValue(int driveNumber);
	int16_t getCurrentActualValue(int driveNumber);
	
	
	
	
// 	uint16_t getStatus(int driveNumber);	//driveNumber is counted from 0
// 	int8_t getModeOfOperation(int driveNumber);	// 1=position 3=velocity 4=torque
// 	int32_t getPosRaw(int driveNumber);
// 	int32_t getVel(int driveNumber);
// 	int16_t getTorque(int driveNumber);
// 	uint16_t getTouchProbeStatus(int driveNumber);
// 	int32_t getTouchProbePos1Positive(int driveNumber);
// 	int32_t getTouchProbePos1Negative(int driveNumber);
// 	int32_t getTouchProbePos2PosValue(int driveNumber);
// 	int32_t getPosAuxRaw(int driveNumber);
// 	
// 	int32_t getPosOffset(int driveNumber);
// 	int32_t getPosAuxOffset(int driveNumber);
// 	
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
	
	
	
	
	
	
// 	private:
	ecmasterlib::EtherCATMain* etherCATStack;
	uint8_t* inBuffer;
	uint8_t* outBuffer;
	
	struct drive {
		int32_t posOffset		= 0;
		int32_t auxPosOffset	= 0;
		int32_t prevRawPos		= 0;
		int32_t prevRawAuxPos	= 0;
		int64_t absPos 			= 0;
		int64_t absAuxPos 		= 0;
	};
	
	drive drives[numberOfDrives];
	
	
	
	touchProbeStateEnum touchProbeState[numberOfDrives];
	
// 		int32_t posAct[numberOfDrives]		= { 0 };
// 		int32_t diff1[numberOfDrives]		= { 0 };
// 		int64_t posNew1[numberOfDrives]		= { 0 };
// 		int32_t posRawOld[numberOfDrives]		= { 0 };
// 		int64_t posOld[numberOfDrives] 		= { 0 };
// 		int32_t posAuxRawOld[numberOfDrives]	= { 0 };
// 		int32_t posAuxOld[numberOfDrives] 		= { 0 };
	int32_t posOffset[numberOfDrives]		= { 0 };
	int32_t posAuxOffset[numberOfDrives]	= { 0 };
	int32_t prevRawPos[numberOfDrives]		= { 0 };
	int32_t prevRawAuxPos[numberOfDrives]	= { 0 };
	int64_t absPos[numberOfDrives] 		= { 0 };
	int64_t absAuxPos[numberOfDrives] 		= { 0 };
	
// 	eeros::logger::Logger log;
	

};








#endif // ETHERCAT_INTERFACE_ELMO_HPP_