#include "coreBlocksFactory.h"

//#include "blocks/beepBlock.h"
#include "details/initialBlock.h"
#include "details/finalBlock.h"
//#include "blocks/nullificationEncoderBlock.h"

#include "details/enginesForwardBlock.h"
#include "details/enginesBackwardBlock.h"
//#include "blocks/enginesStopBlock.h"

#include "details/timerBlock.h"
//#include "blocks/playToneBlock.h"
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

#include "details/waitForTouchSensorBlock.h"
//#include "blocks/waitForSonarDistanceBlock.h"
//#include "blocks/waitForColorBlock.h"
//#include "blocks/waitForColorIntensityBlock.h"
//#include "blocks/waitForLightSensorBlock.h"
//#include "blocks/waitForSoundSensorBlock.h"
//#include "blocks/waitforGyroscopeSensorBlock.h"
//#include "blocks/waitForAccelerometerBlock.h"
//#include "blocks/waitForEncoderBlock.h"
//#include "blocks/waitForButtonsBlock.h"

using namespace interpreterCore::coreBlocks;

CoreBlocksFactory::CoreBlocksFactory(
		qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		)
	: mRobotModelManager(robotModelManager)
	, mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mErrorReporter(errorReporter)
//	, mBlocksTable(blocksTable)
	, mParser(nullptr)
{
}

//RobotsBlockParser * CoreBlocksFactory::getParser()
//{
//	return mParser;
//}

interpreterBase::blocksBase::BlockInterface *CoreBlocksFactory::block(qReal::Id const &element)
{
	interpreterBase::blocksBase::Block * newBlock = nullptr;
	if (elementMetatypeIs(element, "InitialNode")) {
		newBlock = new details::InitialBlock();
	} else if (elementMetatypeIs(element, "FinalNode")) {
		newBlock = new details::FinalBlock();
//	} else if (elementMetatypeIs(element, "Beep")) {
//		newBlock = new BeepBlock(mRobotModel->brick(), *mRobotModel->produceTimer());
	} else if (elementMetatypeIs(element, "Timer")) {
		newBlock = new details::TimerBlock(/*mRobotModel->produceTimer()*/);
	} else if (elementMetatypeIs(element, "WaitForTouchSensor")) {
		newBlock = new details::WaitForTouchSensorBlock(mRobotModelManager.model());
//	} else if (elementMetatypeIs(element, "WaitForSonarDistance")) {
//		newBlock = new WaitForSonarDistanceBlock(mRobotModel);
	} else if (elementMetatypeIs(element, "EnginesForward")) {
		newBlock = new details::EnginesForwardBlock(mRobotModelManager.model());
	} else if (elementMetatypeIs(element, "EnginesBackward")) {
		newBlock = new details::EnginesBackwardBlock(mRobotModelManager.model());
//	} else if (elementMetatypeIs(element, "EnginesStop")) {
//		newBlock = new EnginesStopBlock(mRobotModel->motorA(), mRobotModel->motorB(), mRobotModel->motorC());
//	} else if (elementMetatypeIs(element, "Loop")) {
//		newBlock = new LoopBlock();
//	} else if (elementMetatypeIs(element, "Fork")) {
//		newBlock = new ForkBlock();
//	} else if (elementMetatypeIs(element, "Subprogram")) {
//		newBlock = new SubprogramBlock();
//	} else if (elementMetatypeIs(element, "PlayTone")) {
//		newBlock = new PlayToneBlock(mRobotModel->brick(), *mRobotModel->produceTimer());
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
	}

	newBlock->init(element, mGraphicalModelApi, mLogicalModelApi, &mErrorReporter, mParser, mRobotModelManager);
	return newBlock;
}

bool CoreBlocksFactory::elementMetatypeIs(qReal::Id const &element, QString const &metatype)
{
	return element.type() == qReal::Id("RobotsMetamodel", "RobotsDiagram", metatype);
}

void CoreBlocksFactory::setParser(interpreterBase::blocksBase::BlockParserInterface * const parser)
{
	/// @todo ??? is this ****?
	mParser = parser;
}

qReal::IdList CoreBlocksFactory::providedBlocks() const
{
	qReal::IdList result;

	auto id = [] (QString const &metatype) { return qReal::Id("RobotsMetamodel", "RobotsDiagram", metatype); };

	result
			<< id("InitialNode")
			<< id("FinalNode")
			;

	return result;
}
