#include "enginesStopBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace interpreterCore::coreBlocks::details;

EnginesStopBlock::EnginesStopBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesStopBlock::run()
{
	/// @todo Move to model or some helper.
	QString const ports = stringProperty("Ports");
	QStringList const splitted = ports.split(',', QString::SkipEmptyParts);

	for (QString const &port : splitted) {
		for (interpreterBase::robotModel::PortInfo portInfo : mRobotModel.availablePorts()) {
			if (portInfo.name() == port || portInfo.nameAliases().contains(port)) {
				interpreterBase::robotModel::robotParts::Device *device
						= mRobotModel.configuration().device(portInfo
								, interpreterBase::robotModel::ConfigurationInterface::output);
				interpreterBase::robotModel::robotParts::Motor *motor
						= dynamic_cast<interpreterBase::robotModel::robotParts::Motor *>(device);
				if (motor) {
					motor->off();
				}
			}
		}
	}

	emit done(mNextBlockId);
}
