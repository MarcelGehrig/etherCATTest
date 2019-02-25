#ifndef ETHERCAT_INTERFACE_CONFIG_HPP_
#define ETHERCAT_INTERFACE_CONFIG_HPP_


namespace etherCatInterface {
	

	// Configurations for network
	// ///////////////////////////
	static constexpr int numberOfDrives = 2;
	static constexpr int bytesPerPDOFrame = 34;	// eni.xml --> <EtherCATConfig> <Config> <ProcessImage> <Inputs> <ByteSize>
	static constexpr int bytesPerSlave = bytesPerPDOFrame / numberOfDrives;
	

	// Configurations for Elmo drives
	// //////////////////////////////
	
	enum driveMode_ELMO {
		  profilePosition,
		  profileVelocity,
		  profileTorque,
		  homing,
		  interpolatedPosition,
		  cyclicSynchronousPosition,
		  cyclicSynchronousVelocity,
		  cyclicSynchronousTorque
	  };
	  
	  enum controlWordCommand_ELMO {
		  shutdown,		//0x06
		  switchOn,		//0X07
		  disableDrive,		//0X07 same as switchOn
		  enableOperation,	//0X0F
		  faultReset		//0X80
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
	static constexpr uint16_t switchedOnValue =			0x0023;
	static constexpr uint16_t switchedOnMask =			0x006f;
	static constexpr uint16_t operationEnabledValue = 		0x0027;
	static constexpr uint16_t operationEnabledMask = 		0x006f;
	static constexpr uint16_t quickStopActiveValue =		0x0007;
	static constexpr uint16_t quickStopActiveMask =			0x006f;
	static constexpr uint16_t faultReactionActiveValue =		0x000f;
	static constexpr uint16_t faultReactionActiveMask =		0x004f;
	static constexpr uint16_t faultValue =				0x0008;
	static constexpr uint16_t faultMask =				0x0048;






}

#endif // ETHERCAT_INTERFACE_CONFIG_HPP_