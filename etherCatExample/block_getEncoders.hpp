#ifndef BLOCK_GET_ENCODERS_HPP_
#define BLOCK_GET_ENCODERS_HPP_

#include <eeros/control/Block.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/control/Signal.hpp>
#include <eeros/math/Matrix.hpp>
#include <eeros/core/System.hpp>

#include "../etherCatInterface/EtherCATInterfaceElmo.hpp"


using namespace eeros::control;
using namespace etherCATInterface;

// namespace movingchair {
	
//TODO numberOfDrivesTotal
typedef eeros::math::Matrix< 2 , 1 , double >	typeGetPosition;
// typedef double	typeGetPosition;
// typedef eeros::math::Matrix< numberOfDrivesTotal , 1 , uint16_t >	typeGetStatusWord;
// typedef eeros::math::Matrix< numberOfDrivesTotal , 1 , double >		typeGetPosition;
// // 	typedef eeros::math::Matrix< numberOfDrivesTotal  , 1 , int64_t >		typeGetPositionAux;
// typedef eeros::math::Matrix< numberOfDrivesTotal  , 1 , double >		typeGetPositionAux;
// typedef eeros::math::Matrix< numberOfDrivesTotal , 1 , double > 	typeGetVelocity; // TEST
// typedef eeros::math::Matrix< numberOfDrivesTotal , 1 , double >		typeGetTorque; // TEST 

class block_getEncoders : public Block {
public:
	block_getEncoders(EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal) :
		elmoDrives(elmoDrives),
		numberOfDrivesTotal(numberOfDrivesTotal)
	{
// 		this->statusWord.getSignal().clear();
		this->position.getSignal().clear();
// 		this->position.getSignal().clear();
// 		this->positionAux.getSignal().clear();
// 		this->velocity.getSignal().clear();
// 		this->torque.getSignal().clear();
	}
	
	virtual void run() {
		//statusWord, position, velocity and torque
		for ( int i = 0; i < numberOfDrivesTotal; i++ ) {
// 			statusWordValue(i) = movingChairEtherCAT.getStatus(i);
// 				positionValue(i) = movingChairEtherCAT.getPos(i);
			positionValue(i) = static_cast<double>( elmoDrives.getPos(i) ); 
// 			velocityValue(i) = static_cast<double>( movingChairEtherCAT.getVel(i)); //TEST
// 			torqueValue(i) = static_cast<double>( movingChairEtherCAT.getTorque(i)); // TEST
// 				positionAuxValue(i) = movingChairEtherCAT.getPosAux(i);
// 			positionAuxValue(i) = static_cast<double>( movingChairEtherCAT.getPosAux(i) );
		}
		
		//set outputs
		auto timestamp = eeros::System::getTimeNs();
// 		statusWord.getSignal().setValue(statusWordValue);
// 		statusWord.getSignal().setTimestamp(timestamp);
		
// 		position.getSignal().setValue(elmoDrives.getPos(0));
		position.getSignal().setValue(positionValue);
		position.getSignal().setTimestamp(timestamp);
		
// 		positionAux.getSignal().setValue(positionAuxValue);
// 		positionAux.getSignal().setTimestamp(timestamp);
// 		velocity.getSignal().setValue(velocityValue);
// 		velocity.getSignal().setTimestamp(timestamp);
// 		torque.getSignal().setValue(torqueValue);
// 		torque.getSignal().setTimestamp(timestamp);
		
// 		static int cnt = 0;
		
	}
	
// 	virtual eeros::control::Output< typeGetStatusWord >& getOutStatusWord() {
// 		return statusWord;
// 	}
	
	virtual eeros::control::Output< typeGetPosition >& getOutPosition() {
		return position;
	}
	
// 	virtual eeros::control::Output< typeGetPositionAux >& getOutPositionAux() {
// 		return positionAux;
// 	}
// 	
// 	virtual eeros::control::Output< typeGetVelocity >& getOutVelocity() {
// 		return velocity;
// 	}
// 	
// 	virtual eeros::control::Output< typeGetTorque >& getOutTorque() {
// 		return torque;
// 	}
	
	protected:
// 		typeGetStatusWord                            statusWordValue;
// 		eeros::control::Output< typeGetStatusWord >  statusWord;
		typeGetPosition	                             positionValue;
		eeros::control::Output< typeGetPosition >    position;
// 		typeGetPositionAux				positionAuxValue;
// 		eeros::control::Output< typeGetPositionAux >	positionAux;
// 		typeGetVelocity				velocityValue;
// 		eeros::control::Output< typeGetVelocity >		velocity;
// 		typeGetTorque					torqueValue;
// 		eeros::control::Output< typeGetTorque >		torque;
		
		int numberOfDrivesTotal;
		EtherCATInterfaceElmo& elmoDrives;
		
};
// };


#endif /* BLOCK_GET_ENCODERS_HPP_ */