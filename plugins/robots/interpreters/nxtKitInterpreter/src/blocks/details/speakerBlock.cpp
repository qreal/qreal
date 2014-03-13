#include "speakerBlock.h"

#include <utils/tracer.h>
#include <utils/abstractTimer.h>

/// @todo: use common speaker interface
#include "robotModel/real/parts/speaker.h"

using namespace nxtKitInterpreter::blocks::details;

SpeakerBlock::SpeakerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
	, mTimer(robotModel.produceTimer())
{
	mTimer->setParent(this);
	connect(mTimer, &utils::AbstractTimer::timeout, this, &SpeakerBlock::timeout);
}

SpeakerBlock::~SpeakerBlock()
{
}

void SpeakerBlock::run()
{
	QString const port = "SpeakerPort";
	for (interpreterBase::robotModel::PortInfo const &portInfo : mRobotModel.availablePorts()) {
		if (portInfo.name() == port || portInfo.nameAliases().contains(port)) {
			interpreterBase::robotModel::robotParts::Device *device
					= mRobotModel.configuration().device(portInfo
							, interpreterBase::robotModel::ConfigurationInterface::output);
			robotModel::real::parts::Speaker * const speaker
					= dynamic_cast<robotModel::real::parts::Speaker *>(device);
			if (speaker) {
				doJob(*speaker);
			} else {
				error(tr("Speaker is not configured (WTF?)"));
				return;
			}
		}
	}

	if (!boolProperty("WaitForCompletion"))
		emit done(mNextBlockId);
	else {
		mTimer->start(duration());
	}
}

void SpeakerBlock::timeout()
{
	utils::Tracer::debug(utils::Tracer::blocks, "TimerBlock::timeout", "emit done(mNextBlock)");
	emit done(mNextBlockId);
}
