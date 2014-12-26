#include "speakerBlock.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikSpeaker>(robotModel)
{
}

void SpeakerBlock::doJob(robotModel::parts::TrikSpeaker &speaker)
{
	QString const toSpeak = stringProperty("FileName");
	speaker.play(toSpeak);

	emit done(mNextBlockId);
}
