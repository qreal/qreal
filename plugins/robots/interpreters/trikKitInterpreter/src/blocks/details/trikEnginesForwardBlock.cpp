#include "trikEnginesForwardBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

#include <qrtext/core/error.h>

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

TrikEnginesForwardBlock::TrikEnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::EngineCommandBlock(robotModel)
{
}

void TrikEnginesForwardBlock::run()
{
	int const power = eval<int>("Power");
	QStringList const ports = eval<QStringList>("Ports");
	if (wereParserErrors()) {
		return;
	}

	QList<Motor *> result;

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
