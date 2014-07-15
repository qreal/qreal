#include "ledBlock.h"
#include "src/robotModel/twoD/parts/twoDLed.h"
#include "src/robotModel/parts/trikLed.h"

#include <interpreterBase/blocksBase/common/displayBlock.h>
#include <interpreterBase/robotModel/robotModelUtils.h>
#include <utils/propertyToColor.h>
#include <QColor>

///for LedBlock
using namespace trikKitInterpreter;
using namespace blocks;
using namespace details;

///for RobotModelInterface
using namespace interpreterBase::robotModel;

///for color choise
using namespace utils;



LedBlock::LedBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

LedBlock::~LedBlock()
{
}

void LedBlock::run()
{
	QColor color = utils::PropertyToColor::propertyToColor(stringProperty("Color"));

	QString const port = "LedPort";
	robotModel::parts::TrikLed * const led
			= RobotModelUtils::findDevice<robotModel::parts::TrikLed>(mRobotModel, port);

	if (led) {
		led->setColor(color);
	} else {
		error(tr("Led is not configured (WTF?)"));
	}


	emit done(mNextBlockId);
}

