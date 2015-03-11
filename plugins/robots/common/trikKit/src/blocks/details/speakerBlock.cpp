#include "speakerBlock.h"

using namespace trik::blocks::details;
using namespace kitBase::robotModel;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikSpeaker>(robotModel)
{
}

void SpeakerBlock::doJob(robotModel::parts::TrikSpeaker &speaker)
{
	const QString toSpeak = stringProperty("FileName");
	speaker.play(toSpeak);

	emit done(mNextBlockId);
}
