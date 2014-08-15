#include "trikEnginesForwardBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

TrikEnginesForwardBlock::TrikEnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::EngineCommandBlock(robotModel)
{
}

void TrikEnginesForwardBlock::run()
{
	int const power = mNewParser->interpret<int>(mGraphicalId, "Power", stringProperty("Power"));

	QList<Motor *> result;
	QStringList const ports = mNewParser->interpret<QStringList>(mGraphicalId, "Ports", stringProperty("Ports"));

	for (QString const &port : ports) {
		Motor * const motor =
				interpreterBase::robotModel::RobotModelUtils::findDevice<Motor>(mRobotModel, port.trimmed());

		if (motor) {
			result << motor;
		}
	}

	for (Motor * const motor : result) {
		motor->on(power);
	}

	emit done(mNextBlockId);
}
