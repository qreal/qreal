#include "enginesForwardBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

//#include "../tracer.h"

using namespace interpreterCore::coreBlocks::details;

EnginesForwardBlock::EnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesForwardBlock::run()
{
//	Tracer::debug(tracer::enums::blocks, "EnginesForwardBlock::run", "");
	int const power = evaluate("Power").toInt();

	/// @todo Makes sense only for NXT.
	bool const breakMode = stringProperty("Mode") != QString::fromUtf8("скользить");

	/// @todo Move to model or some helper.
	QString const ports = stringProperty("Ports");
	QStringList const splitted = ports.split(',', QString::SkipEmptyParts);

	for (QString const &port : splitted) {
		for (interpreterBase::robotModel::PortInfo portInfo : mRobotModel.availablePorts()) {
			if (portInfo.name() == port || portInfo.nameAliases().contains(port)) {
				interpreterBase::robotModel::robotParts::PluggableDevice *device
						= mRobotModel.configuration().pluggableDevice(portInfo
								, interpreterBase::robotModel::ConfigurationInterface::output);
				interpreterBase::robotModel::robotParts::Motor *motor
						= static_cast<interpreterBase::robotModel::robotParts::Motor *>(device);
				if (motor != nullptr) {
					motor->on(power, breakMode);
				}
			}
		}
	}

	emit done(mNextBlockId);
}
