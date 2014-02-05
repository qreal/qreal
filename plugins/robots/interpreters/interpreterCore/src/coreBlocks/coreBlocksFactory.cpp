#include "coreBlocksFactory.h"

#include "details/finalBlock.h"
#include "details/initialBlock.h"
//#include "details/timerBlock.h"
//#include "details/beepBlock.h"
//#include "details/nullificationEncoderBlock.h"

//#include "details/enginesForwardBlock.h"
//#include "details/enginesBackwardBlock.h"
//#include "details/enginesStopBlock.h"

//#include "details/timerBlock.h"
//#include "details/playToneBlock.h"
//#include "details/functionBlock.h"
//#include "details/beepBlock.h"

//#include "details/loopBlock.h"
//#include "details/forkBlock.h"
//#include "details/ifBlock.h"
//#include "details/dummyBlock.h"
//#include "details/commentBlock.h"
//#include "details/subprogramBlock.h"

//#include "details/clearScreenBlock.h"
//#include "details/drawPixelBlock.h"
//#include "details/drawLineBlock.h"
//#include "details/drawRectBlock.h"
//#include "details/drawCircleBlock.h"
//#include "details/printTextBlock.h"

//#include "details/waitForTouchSensorBlock.h"
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

using namespace qReal;
using namespace interpreterBase::robotModel;
using namespace interpreterBase::blocksBase;

//BlocksFactory::BlocksFactory(GraphicalModelAssistInterface const &graphicalModelApi
//		, LogicalModelAssistInterface const &logicalModelApi
//		, RobotModelInterface * const robotModel
//		, ErrorReporterInterface * const errorReporter
//		)
//		: mRobotModel(robotModel)
//		, mGraphicalModelApi(graphicalModelApi)
//		, mLogicalModelApi(logicalModelApi)
//		, mErrorReporter(errorReporter)
//		, mParser(NULL)
//{
//}

void CoreBlocksFactory::setParser(BlockParserInterface * const parser)
{
//	// TODO: make sure there is no parser already.
//	mParser = parser;
}

////RobotsBlockParser * BlocksFactory::getParser()
////{
////	return mParser;
////}

BlockInterface *CoreBlocksFactory::block(Id const &element)
{
	Block * newBlock = nullptr;
//	if (elementMetatypeIs(element, "InitialNode")) {
//		newBlock = new details::InitialBlock(*mRobotModel);
//	} else if (elementMetatypeIs(element, "FinalNode")) {
//		newBlock = new details::FinalBlock();
//	} else if (elementMetatypeIs(element, "Beep")) {
//		newBlock = new BeepBlock(mRobotModel->brick(), mRobotModel->produceTimer());
//	} else if (elementMetatypeIs(element, "Timer")) {
//		newBlock = new TimerBlock(mRobotModel->produceTimer());
//	} else if (elementMetatypeIs(element, "WaitForTouchSensor")) {
//		newBlock = new WaitForTouchSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForSonarDistance")) {
//		newBlock = new WaitForSonarDistanceBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "EnginesForward")) {
//		newBlock = new EnginesForwardBlock(mRobotModel->motorA(), mRobotModel->motorB(), mRobotModel->motorC());
//	} else if (elementMetatypeIs(element, "EnginesBackward")) {
//		newBlock = new EnginesBackwardBlock(mRobotModel->motorA(), mRobotModel->motorB(), mRobotModel->motorC());
//	} else if (elementMetatypeIs(element, "EnginesStop")) {
//		newBlock = new EnginesStopBlock(mRobotModel->motorA(), mRobotModel->motorB(), mRobotModel->motorC());
//	} else if (elementMetatypeIs(element, "Loop")) {
//		newBlock = new LoopBlock();
//	} else if (elementMetatypeIs(element, "Fork")) {
//		newBlock = new ForkBlock();
//	} else if (elementMetatypeIs(element, "Subprogram")) {
//		newBlock = new SubprogramBlock();
//	} else if (elementMetatypeIs(element, "PlayTone")) {
//		newBlock = new PlayToneBlock(mRobotModel->brick());
//	} else if (elementMetatypeIs(element, "Function")) {
//		newBlock = new FunctionBlock();
//	} else if (elementMetatypeIs(element, "WaitForColor")) {
//		newBlock = new WaitForColorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForColorIntensity")) {
//		newBlock = new WaitForColorIntensityBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "IfBlock")) {
//		newBlock = new IfBlock();
//	} else if (elementMetatypeIs(element, "WaitForEncoder")) {
//		newBlock = new WaitForEncoderBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "NullificationEncoder")) {
//		newBlock = new NullificationEncoderBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForLight")) {
//		newBlock = new WaitForLightSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForSound")) {
//		newBlock = new WaitForSoundSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "WaitForGyroscope")) {
//		newBlock = new WaitForGyroscopeSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element,"WaitForAccelerometer")) {
//		newBlock = new WaitForAccelerometerSensorBlock(mRobotModel);
//	} else if (elementMetatypeIs(element, "CommentBlock")) {
//		newBlock = new CommentBlock();
//	} else if (elementMetatypeIs(element, "WaitForButtons")) {
//		newBlock = new WaitForButtonsBlock(mRobotModel, mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawPixel")) {
//		newBlock = new DrawPixelBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawLine")) {
//		newBlock = new DrawLineBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawCircle")) {
//		newBlock = new DrawCircleBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "PrintText")) {
//		newBlock = new PrintTextBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "DrawRect")) {
//		newBlock = new DrawRectBlock(mRobotModel->display());
//	} else if (elementMetatypeIs(element, "ClearScreen")) {
//		newBlock = new ClearScreenBlock(mRobotModel->display());
//	} else {
//		newBlock = new DummyBlock();
//	}

//	newBlock->init(element, mGraphicalModelApi, mLogicalModelApi, mErrorReporter, mParser);
	return newBlock;
}

//bool BlocksFactory::elementMetatypeIs(Id const &element, QString const &metatype)
//{
//	return element.type() == id(metatype);
//}

qReal::IdList CoreBlocksFactory::providedBlocks() const
{
	return IdList()
//			<< id("InitialNode")
//			<< id("FinalNode")
//			<< id("Beep")
//			<< id("Timer")
//			<< id("EnginesForward")
//			<< id("EnginesBackward")
//			<< id("EnginesStop")
//			<< id("Loop")
//			<< id("Fork")
//			<< id("Subprogram")
//			<< id("Function")
//			<< id("IfBlock")
//			<< id("CommentBlock")
			;
}

//qReal::Id BlocksFactory::id(QString const &metatype)
//{
//	return Id("RobotsMetamodel", "RobotsDiagram", metatype);
//}
