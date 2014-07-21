#include "ledBlock.h"

#include <QtGui/QColor>

#include <interpreterBase/blocksBase/common/displayBlock.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/twoD/parts/twoDLed.h"
#include "src/robotModel/parts/trikLed.h"

using namespace trikKitInterpreter;
using namespace blocks;
using namespace details;
using namespace interpreterBase::robotModel;
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
	QString const stringColor = stringProperty("Color");
	QColor const color = stringColor == "off" ? QColor(Qt::gray) : QColor(stringColor);

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

