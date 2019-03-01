#ifndef ETHERCAT_INTERFACE_ELMO_CONFIG_HPP_
#define ETHERCAT_INTERFACE_ELMO_CONFIG_HPP_

#include "EtherCATInterfaceBase_config.hpp"

namespace etherCATInterface {
	
	// Configurations offsets in PDOs (ENI files)
	// //////////////////////////////////////////
		
	// Output Offsets (oo) PDOs (output from master) [in Byte]
	// Elmo: GoldLine "EtherCAT Application Manual.pdf"     p. 21
	static constexpr int oo_controlWord				= 8;
	static constexpr int oo_modeOfOperation			= -1;
	static constexpr int oo_targetTorque			= -1;
	static constexpr int oo_maxTorque				= -1;
	static constexpr int oo_maxCurrent				= -1;
	static constexpr int oo_targetPostition			= 0;
	static constexpr int oo_maxProfileVelocity		= -1;
	static constexpr int oo_profileVelocity			= -1;
	static constexpr int oo_endVelocity				= -1;
	static constexpr int oo_profileAcceleration		= -1;
	static constexpr int oo_profileDeceleration		= -1;
	static constexpr int oo_torqueSlope				= -1;
	static constexpr int oo_positionOffset			= -1;
	static constexpr int oo_velocityOffset			= -1;
	static constexpr int oo_torqueOffset			= -1;
	static constexpr int oo_touchProbeFunction		= -1;
	static constexpr int oo_interpolatedDataRecord_1		= -1;
	static constexpr int oo_interpolatedDataRecord_2		= -1;
	static constexpr int oo_targetVelocity			= -1;
	static constexpr int oo_digitalOutput			= 4;
	static constexpr int oo_polarity				= -1;
	
	static constexpr int oo_gainSchedlingManualIndex = -1;
	
	
	// Input Offsets (io) PDOs [in Byte]
	// Elmo: GoldLine "EtherCAT Application Manual.pdf"     p. 23
	static constexpr int io_statusWord					= 8;
	static constexpr int io_modeOfOperationDisplay		= -1;
	static constexpr int io_postionDemand_UU			= -1;
	static constexpr int io_actualPosition_counts		= -1;
	static constexpr int io_positionActualValue			= 0;
	static constexpr int io_velocitySensorActualValue	= -1;
	static constexpr int io_velocityDemand				= -1;
	static constexpr int io_velocityActualValue			= -1;
	static constexpr int io_torqueDemandValue			= -1;
	static constexpr int io_torqueActualValue			= -1;
	static constexpr int io_touchProbeStatus			= -1;
	static constexpr int io_touchProbePos1Positive		= -1;
	static constexpr int io_touchProbePos1Negative		= -1;
	static constexpr int io_touchProbePos2Positive		= -1;
	static constexpr int io_DCLinkCircuitVoltage		= -1;
	static constexpr int io_positionFollowingError		= -1;
	static constexpr int io_controllEffort				= -1;
	static constexpr int io_positionDemandValue_cnt		= -1;
	static constexpr int io_digitalInputs				= 4;
	static constexpr int io_analogInput					= -1;
	static constexpr int io_auxilaryPositionActualValue	= -1;
	static constexpr int io_currentActualValue			= -1;
	

	// Configurations for Elmo drives
	// //////////////////////////////
	
	enum driveModeOfOperation_ELMO {
		CANEncoderMode,
		profilePosition,
		profileVelocity,
		profileTorque,
		homing,
		interpolatedPosition,
		cyclicSynchronousPosition,
		cyclicSynchronousVelocity,
		cyclicSynchronousTorque
	};
	
	// drive mode of operations value (dmoov)
	static constexpr int16_t dmoov_CANEncoderMode				= -3;
	static constexpr int16_t dmoov_profilePosition				= 1;
	static constexpr int16_t dmoov_profileVelocity				= 3;
	static constexpr int16_t dmoov_profileTorque				= 4;
	static constexpr int16_t dmoov_homing						= 6;
	static constexpr int16_t dmoov_interpolatedPosition			= 7;
	static constexpr int16_t dmoov_cyclicSynchronousPosition	= 8;
	static constexpr int16_t dmoov_cyclicSynchronousVelocity	= 9;
	static constexpr int16_t dmoov_cyclicSynchronousTorque		= 10;
	
	
	    
	 
	  enum controlWordCommand_ELMO {
		shutdown,			//0x06
		switchOn,			//0X07
		switchOnAndEnable,	//0x0F
		disableVoltage,		//0x00
		quickStop,			//0x02
		disableOperation,	//0x07
		enableOperation,	//0x0F
		faultReset			//0X80
	  };
	  
	// Control World Commands (cwc)
	static constexpr uint16_t cwc_shutdown			= 0x06;
	static constexpr uint16_t cwc_switchOn			= 0x07;
	static constexpr uint16_t cwc_switchOnAndEnable	= 0x0F;
	static constexpr uint16_t cwc_disableVoltage	= 0x00;
	static constexpr uint16_t cwc_quickStop			= 0x02;
	static constexpr uint16_t cwc_disableOperation	= 0x07;
	static constexpr uint16_t cwc_enableOperation	= 0x0F;
	static constexpr uint16_t cwc_faultReset		= 0x80;
	  
	
	// Drive states (elmo "CAN DS-402 Implementation Guide" p. 46
	enum driveStatus_ELMO {
		notReadyToSwitchOn,
		switchOnDisabled,
		readyToSwitchOn,
		switchedOn,
		operationEnabled,
		quickStopActive,
		faultReactionactive,
		fault
	};
	
	enum touchProbeStateEnum_ELMO {
		  reset,
		  enableProbe,
		  enableSampling,
		  enabled
	  };

	static constexpr uint16_t notReadyToSwitchOnValue =		0x0000;
	static constexpr uint16_t notReadyToSwitchOnMask =		0x004f;
	static constexpr uint16_t switchOnDisabledValue =		0x0040;
	static constexpr uint16_t switchOnDisabledMask =		0x004f;
	static constexpr uint16_t readyToSwitchOnValue =		0x0021;
	static constexpr uint16_t readyToSwitchOnMask = 		0x006f;
	static constexpr uint16_t switchedOnValue =				0x0023;
	static constexpr uint16_t switchedOnMask =				0x006f;
	static constexpr uint16_t operationEnabledValue = 		0x0027;
	static constexpr uint16_t operationEnabledMask = 		0x006f;
	static constexpr uint16_t quickStopActiveValue =		0x0007;
	static constexpr uint16_t quickStopActiveMask =			0x006f;
	static constexpr uint16_t faultReactionActiveValue =	0x000f;
	static constexpr uint16_t faultReactionActiveMask =		0x004f;
	static constexpr uint16_t faultValue =					0x0008;
	static constexpr uint16_t faultMask =					0x0048;


	// switch to different set of gains for velocity/torque 
	static constexpr int gainSchedulingManualIndex_position =	-1;		// chair 1
	static constexpr int gainSchedulingManualIndex_velocity =	-1;		// chair 1
	static constexpr int gainSchedulingManualIndex_torque =		-1;		// chair 2
	static constexpr int gainSchedulingManualIndex_homing =		-1;		// chair 2




}

#endif // ETHERCAT_INTERFACE_ELMO_CONFIG_HPP_