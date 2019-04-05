#ifndef BLOCK_SET_ELMOS_HPP
#define BLOCK_SET_ELMOS_HPP

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Signal.hpp>
#include <eeros/math/Matrix.hpp>
#include <eeros/core/System.hpp>

#include "../etherCatInterface/EtherCATInterfaceElmo.hpp"
#include "globalConfig.hpp"
#include </home/ubuntu/eeros/includes/eeros/control/Input.hpp>


using namespace eeros::control;
using namespace etherCATInterface;

//TODO numberOfDrivesTotal
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeDigitalOutput;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint16_t >		typeGetStatusWord;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int8_t >		typeGetModeOfOperation;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetPosition;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetPositionAux;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetVelocity;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetTorque;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetTouchProbePos1Positive;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetTouchProbePos1Negative;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetTouchProbePos2Positive;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetDCLinkCircuitVoltage;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetPositionFollowingError;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetPositionControlEffort;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetPositionDemandValue;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , uint32_t >	typeGetDigitalInputs;
// typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1 , double >		typeGetAnalogInput;


// controlWord
// modeOfOperation
// targetTorque
// maxTorque
// maxCurrent
// targetPosition
// maxProfileVelocity
// profileVelocity
// endVelocity
// profileAcceleration
// profileDeceleration
// torqueSlope
// positionOffset
// velocityOffset
// torqueOffset
// touchProbeFunction
// interpolatedDataRecord1
// interpolatedDataRecord2
// targetVelocity
// digitalOutput
// polarity
// index


// uint16_t controlWord
// int8_t modeOfOperation
// int16_t targetTorque
// int16_t maxTorque
// int16_t maxCurrent
// int32_t targetPosition
// uint32_t maxProfileVelocity
// uint32_t profileVelocity
// uint32_t endVelocity
// uint32_t profileAcceleration
// uint32_t profileDeceleration
// uint32_t torqueSlope
// int32_t positionOffset
// int32_t velocityOffset
// int16_t torqueOffset
// uint16_t touchProbeFunction
// int32_t interpolatedDataRecord1
// int32_t interpolatedDataRecord2
// int32_t targetVelocity
// uint32_t digitalOutput
// uint8_t polarity
// uint16_t index
		
		
class block_setElmos : public Block {
public:
	block_setElmos(EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal) :
		elmoDrives(elmoDrives),
		numberOfDrivesTotal(numberOfDrivesTotal)
	{
// 		statusWord.getSignal().clear();
// 		modeOfOperation.getSignal().clear();
// 		position.getSignal().clear();
// 		positionAux.getSignal().clear();
// 		velocity.getSignal().clear();
// 		torque.getSignal().clear();
// 		touchProbePos1Positive.getSignal().clear();
// 		touchProbePos1Negative.getSignal().clear();
// 		touchProbePos2Positive.getSignal().clear();
// 		dCLinkCircuitVoltage.getSignal().clear();
// 		positionFollowingError.getSignal().clear();
// 		positionControlEffort.getSignal().clear();
// 		positionDemandValue.getSignal().clear(); 
// 		digitalInputs.getSignal().clear();
// 		analogInput.getSignal().clear();
	}
	
	virtual void run() {
		// get PDO values from EtherCAT using EtherCATInterfaceElmo
		//  if the input value is not sent by PDO (Interface_config: io_... < 0) the variable will not be processed
// 		for ( int i = 0; i < numberOfDrivesTotal; i++ ) {
// 			if( etherCATInterface::io_statusWord >= 0 )
// 				statusWordValue(i) = elmoDrives.ll_getStatusWord(i);
// 			if( etherCATInterface::io_modeOfOperationDisplay >= 0 )
// 				modeOfOperationValue(i) = elmoDrives.ll_getModeOfOperationDisplay(i);
// 			if( etherCATInterface::io_positionActualValue >= 0 )
// 				positionValue(i) = static_cast<double>( elmoDrives.getPosition(i) );
// 			if( etherCATInterface::io_auxilaryPositionActualValue >= 0 )
// 				positionAuxValue(i) = static_cast<double>( elmoDrives.getPositionAux(i) );
// 			if( etherCATInterface::io_velocityActualValue >= 0 )
// 				velocityValue(i) = static_cast<double>( elmoDrives.ll_getVelocityActualValue(i) );
// 			if( etherCATInterface::io_velocityActualValue >= 0 )
// 				torqueValue(i) = static_cast<double>( elmoDrives.ll_getTorqueActualValue(i) );
// 			if( etherCATInterface::io_touchProbePos1Positive >= 0 )
// 				touchProbePos1PositiveValue(i) = static_cast<double>( elmoDrives.ll_getTouchProbePos1Positive(i) );
// 			if( etherCATInterface::io_touchProbePos1Negative >= 0 )
// 				touchProbePos1NegativeValue(i) = static_cast<double>( elmoDrives.ll_getTouchProbePos1Negative(i) );
// 			if( etherCATInterface::io_touchProbePos2Positive >= 0 )
// 				touchProbePos2PositiveValue(i) = static_cast<double>( elmoDrives.ll_getTouchProbePos2Positive(i) );
// 			if( etherCATInterface::io_DCLinkCircuitVoltage >= 0 )
// 				dCLinkCircuitVoltageValue(i) = static_cast<double>( elmoDrives.ll_getDCLinkCircuitVoltage(i) );
// 			if( etherCATInterface::io_positionFollowingError >= 0 )
// 				positionFollowingErrorValue(i) = static_cast<double>( elmoDrives.ll_getPositionFollowingError(i) );
// 			if( etherCATInterface::io_controlEffort >= 0 )
// 				positionControlEffortValue(i) = static_cast<double>( elmoDrives.ll_getControlEffort(i) );
// 			if( etherCATInterface::io_positionDemandValue_cnt >= 0 )
// 				positionDemandValueValue(i) = static_cast<double>( elmoDrives.ll_getPositionDemandValue_cnt(i) );
// 			if( etherCATInterface::io_digitalInputs >= 0 )
// 				digitalInputsValue(i) = elmoDrives.ll_getDigitalInputs(i);
// 			if( etherCATInterface::io_analogInput >= 0 )
// 				analogInputValue(i) = static_cast<double>( elmoDrives.ll_getAnalogInput(i) );
// 		}
		
		for ( int i = 0; i < numberOfDrivesTotal; i++ ) {
			if( etherCATInterface::oo_digitalOutput >= 0 )
// 				std::cout << inDigitalOutput.getSignal().getValue()(i) << std::endl;
// 				log.info() << "asdf";
				elmoDrives.ll_setDigitalOutput(i, inDigitalOutput.getSignal().getValue()(i));
		}
		
		
	}
	
	
	// Defining getIn...() methods
		virtual eeros::control::Input<typeDigitalOutput>& getInDigitalOutput() {
			return inDigitalOutput;
		}
	
	
// 	// Defining getOut...() methods
// 	//  if the input value is not sent by PDO (Interface_config: io_... < 0) and the output is connectetd in the CS, eeros thows a fault
// 	virtual eeros::control::Output< typeGetStatusWord >& getOutStatusWord() {
// 		if( etherCATInterface::io_statusWord >= 0 )
// 			return statusWord;
// 		else {
// 			throw eeros::Fault( "PDO offset for statusWord not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 		}
// 	}
// 	virtual eeros::control::Output< typeGetPosition >& getOutPosition() {
// 		if( etherCATInterface::io_positionActualValue >= 0 )
// 			return position;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetPositionAux >& getOutPositionAux() {
// 		if( etherCATInterface::io_auxilaryPositionActualValue >= 0 )
// 			return positionAux;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetVelocity >& getOutVelocity() {
// 		if( etherCATInterface::io_velocityActualValue >= 0 )
// 			return velocity;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetTorque >& getOutTorque() {
// 		if( etherCATInterface::io_torqueActualValue >= 0 )
// 			return torque;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetTouchProbePos1Positive >& getOutTouchProbePos1Positive() {
// 		if( etherCATInterface::io_touchProbePos1Positive >= 0 )
// 			return touchProbePos1Positive;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetTouchProbePos1Negative >& getOutTouchProbePos1Negative() {
// 		if( etherCATInterface::io_touchProbePos1Negative >= 0 )
// 			return touchProbePos1Negative;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetTouchProbePos2Positive >& getOutTouchProbePos2Positive() {
// 		if( etherCATInterface::io_touchProbePos2Positive >= 0 )
// 			return touchProbePos2Positive;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetDCLinkCircuitVoltage >& getOutDCLinkCircuitVoltage() {
// 		if( etherCATInterface::io_DCLinkCircuitVoltage >= 0 )
// 			return dCLinkCircuitVoltage;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetPositionFollowingError >& getOutPositionFollowingError() {
// 		if( etherCATInterface::io_positionFollowingError >= 0 )
// 			return positionFollowingError;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetPositionControlEffort >& getOutPositionControlEffort() {
// 		if( etherCATInterface::io_controlEffort >= 0 )
// 			return positionControlEffort;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetPositionDemandValue >& getOutPositionDemandValue() {
// 		if( etherCATInterface::io_positionDemandValue_cnt >= 0 )
// 			return positionDemandValue;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetDigitalInputs >& getOutDigitalInputs() {
// 		if( etherCATInterface::io_digitalInputs >= 0 )
// 			return digitalInputs;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
// 	virtual eeros::control::Output< typeGetAnalogInput >& getOutAnalogInput() {
// 		if( etherCATInterface::io_analogInput >= 0 )
// 			return analogInput;
// 		else
// 			throw eeros::Fault( "PDO offset for position not defined. Define offset in EtherCATInterfaceElmo_config.hpp" );
// 	}
	
	
	protected:
//TODO numberOfDrivesTotal
		
// 		typeGetStatusWord											statusWordValue;
// 		eeros::control::Output< typeGetStatusWord >					statusWord;
// 		typeGetModeOfOperation										modeOfOperationValue;
// 		eeros::control::Output< typeGetModeOfOperation >			modeOfOperation;
// 		typeGetPosition												positionValue;
// 		eeros::control::Output< typeGetPosition >					position;
// 		typeGetPositionAux											positionAuxValue;
// 		eeros::control::Output< typeGetPositionAux >				positionAux;
// 		typeGetVelocity												velocityValue;
// 		eeros::control::Output< typeGetVelocity >					velocity;
// 		typeGetTorque												torqueValue;
// 		eeros::control::Output< typeGetTorque >						torque;
// 		typeGetTouchProbePos1Positive								touchProbePos1PositiveValue;
// 		eeros::control::Output< typeGetTouchProbePos1Positive >		touchProbePos1Positive;
// 		typeGetTouchProbePos1Negative								touchProbePos1NegativeValue;
// 		eeros::control::Output< typeGetTouchProbePos1Negative >		touchProbePos1Negative;
// 		typeGetTouchProbePos2Positive								touchProbePos2PositiveValue;
// 		eeros::control::Output< typeGetTouchProbePos2Positive >		touchProbePos2Positive;
// 		typeGetDCLinkCircuitVoltage									dCLinkCircuitVoltageValue;
// 		eeros::control::Output< typeGetDCLinkCircuitVoltage >		dCLinkCircuitVoltage;
// 		typeGetPositionFollowingError								positionFollowingErrorValue;
// 		eeros::control::Output< typeGetPositionFollowingError >		positionFollowingError;
// 		typeGetPositionControlEffort								positionControlEffortValue;
// 		eeros::control::Output< typeGetPositionControlEffort >		positionControlEffort;
// 		typeGetPositionDemandValue									positionDemandValueValue;
// 		eeros::control::Output< typeGetPositionDemandValue >		positionDemandValue;
// 		typeGetDigitalInputs										digitalInputsValue;
// 		eeros::control::Output< typeGetDigitalInputs >				digitalInputs;
// 		typeGetAnalogInput											analogInputValue;
// 		eeros::control::Output< typeGetAnalogInput >				analogInput;
		
		eeros::control::Input< typeDigitalOutput >		inDigitalOutput;
		
		int numberOfDrivesTotal;
		EtherCATInterfaceElmo& elmoDrives;
		
};

#endif /* BLOCK_SET_ELMOS_HPP */
