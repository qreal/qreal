#include "coreBlocksFactory.h"

#include "details/initialBlock.h"
#include "details/finalBlock.h"
//#include "details/nullificationEncoderBlock.h"

#include "details/enginesForwardBlock.h"
#include "details/enginesBackwardBlock.h"
#include "details/enginesStopBlock.h"

#include "details/timerBlock.h"
//#include "details/functionBlock.h"

//#include "details/loopBlock.h"
//#include "details/forkBlock.h"
//#include "details/ifBlock.h"
//#include "details/dummyBlock.h"
#include "details/commentBlock.h"
//#include "details/subprogramBlock.h"

#include "details/waitForTouchSensorBlock.h"
//#include "details/waitForSonarDistanceBlock.h"
//#include "details/waitForColorBlock.h"
//#include "details/waitForColorIntensityBlock.h"
//#include "details/waitForLightSensorBlock.h"
//#include "details/waitForSoundSensorBlock.h"
//#include "details/waitforGyroscopeSensorBlock.h"
//#include "details/waitForAccelerometerBlock.h"
//#include "details/waitForEncoderBlock.h"
//#include "details/waitForButtonsBlock.h"

using namespace interpreterCore::coreBlocks;

interpreterBase::blocksBase::Block *CoreBlocksFactory::produceBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "InitialNode")) {
		return new details::InitialBlock();
	} else if (elementMetatypeIs(element, "FinalNode")) {
		return new details::FinalBlock();
	} else if (elementMetatypeIs(element, "Timer")) {
		return new details::TimerBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "EnginesForward")) {
		return new details::EnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "EnginesBackward")) {
		return new details::EnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "EnginesStop")) {
		return new details::EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "CommentBlock")) {
		return new details::CommentBlock;
//	} else if (elementMetatypeIs(element, "IfBlock")) {
//		return new IfBlock();
//	} else if (elementMetatypeIs(element, "Loop")) {
//		return new LoopBlock();
//	} else if (elementMetatypeIs(element, "Fork")) {
//		return new ForkBlock();
//	} else if (elementMetatypeIs(element, "Subprogram")) {
//		return new SubprogramBlock();
//	} else if (elementMetatypeIs(element, "Function")) {
//		return new FunctionBlock();
//	} else if (elementMetatypeIs(element, "NullificationEncoder")) {
//		return new details::NullificationEncoderBlock(mRobotModel);
	} else if (elementMetatypeIs(element, "WaitForTouchSensor")) {
		return new details::WaitForTouchSensorBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "WaitForButtons")) {
//		return new WaitForButtonsBlock(mRobotModel, mRobotModel->display());
//	} else {
//		return new DummyBlock();
	}

	return nullptr;
}

qReal::IdList CoreBlocksFactory::providedBlocks() const
{
	return {
		id("InitialNode")
		, id("FinalNode")
	};
}
