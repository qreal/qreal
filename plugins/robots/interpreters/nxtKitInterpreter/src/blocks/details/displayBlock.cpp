#include "displayBlock.h"

#include <utils/tracer.h>

using namespace nxtKitInterpreter::blocks::details;

DisplayBlock::DisplayBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

DisplayBlock::~DisplayBlock()
{
}

void DisplayBlock::run()
{
	QString const port = "DisplayPort";
	for (interpreterBase::robotModel::PortInfo const &portInfo : mRobotModel.availablePorts()) {
		if (portInfo.name() == port || portInfo.nameAliases().contains(port)) {
			interpreterBase::robotModel::robotParts::Device *device
					= mRobotModel.configuration().device(portInfo
							, interpreterBase::robotModel::ConfigurationInterface::output);
			robotModel::parts::NxtDisplay * const display
					= dynamic_cast<robotModel::parts::NxtDisplay *>(device);
			if (display) {
				doJob(*display);
			} else {
				error(tr("Display is not configured (WTF?)"));
				return;
			}
		}
	}
}
