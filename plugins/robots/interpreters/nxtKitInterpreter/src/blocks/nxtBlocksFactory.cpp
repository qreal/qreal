#include "nxtBlocksFactory.h"

#include "details/beepBlock.h"
#include "details/playToneBlock.h"
//#include "blocks/nullificationEncoderBlock.h"

//#include "blocks/enginesStopBlock.h"

//#include "blocks/functionBlock.h"
//#include "blocks/beepBlock.h"

//#include "blocks/loopBlock.h"
//#include "blocks/forkBlock.h"
//#include "blocks/ifBlock.h"
//#include "blocks/dummyBlock.h"
//#include "blocks/commentBlock.h"
//#include "blocks/subprogramBlock.h"

//#include "blocks/clearScreenBlock.h"
//#include "blocks/drawPixelBlock.h"
//#include "blocks/drawLineBlock.h"
//#include "blocks/drawRectBlock.h"
//#include "blocks/drawCircleBlock.h"
//#include "blocks/printTextBlock.h"

//#include "blocks/waitForSonarDistanceBlock.h"
//#include "blocks/waitForColorBlock.h"
//#include "blocks/waitForColorIntensityBlock.h"
//#include "blocks/waitForLightSensorBlock.h"
//#include "blocks/waitForSoundSensorBlock.h"
//#include "blocks/waitforGyroscopeSensorBlock.h"
//#include "blocks/waitForAccelerometerBlock.h"
//#include "blocks/waitForEncoderBlock.h"
//#include "blocks/waitForButtonsBlock.h"

using namespace nxtKitInterpreter::blocks;
using namespace details;

interpreterBase::blocksBase::Block *NxtBlocksFactory::produceBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "Beep")) {
		return new BeepBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "WaitForSonarDistance")) {
//		return new WaitForSonarDistanceBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "EnginesStop")) {
//		return new EnginesStopBlock(mRobotModel->motorA(), mRobotModel->motorB(), mRobotModel->motorC());
//	} else if (elementMetatypeIs(element, "Loop")) {
//		return new LoopBlock();
//	} else if (elementMetatypeIs(element, "Fork")) {
//		return new ForkBlock();
//	} else if (elementMetatypeIs(element, "Subprogram")) {
//		return new SubprogramBlock();
	} else if (elementMetatypeIs(element, "PlayTone")) {
		return new PlayToneBlock(mRobotModelManager->model());
//	} else if (elementMetatypeIs(element, "Function")) {
//		return new FunctionBlock();
//	} else if (elementMetatypeIs(element, "WaitForColor")) {
//		return new WaitForColorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForColorIntensity")) {
//		return new WaitForColorIntensityBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "IfBlock")) {
//		return new IfBlock();
//	} else if (elementMetatypeIs(element, "WaitForEncoder")) {
//		return new WaitForEncoderBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "NullificationEncoder")) {
//		return new NullificationEncoderBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForLight")) {
//		return new WaitForLightSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForSound")) {
//		return new WaitForSoundSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForGyroscope")) {
//		return new WaitForGyroscopeSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element,"WaitForAccelerometer")) {
//		return new WaitForAccelerometerSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "CommentBlock")) {
//		return new CommentBlock();
//	} else if (elementMetatypeIs(element, "WaitForButtons")) {
//		return new WaitForButtonsBlock(mRobotModel, mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawPixel")) {
//		return new DrawPixelBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawLine")) {
//		return new DrawLineBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawCircle")) {
//		return new DrawCircleBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "PrintText")) {
//		return new PrintTextBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawRect")) {
//		return new DrawRectBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "ClearScreen")) {
//		return new ClearScreenBlock(mRobotModel->display());
//	} else {
//		return new DummyBlock();
	}

	return nullptr;
}

qReal::IdList NxtBlocksFactory::providedBlocks() const
{
	return {
		id("Beep")
		, id("PlayTone")
	};
}
