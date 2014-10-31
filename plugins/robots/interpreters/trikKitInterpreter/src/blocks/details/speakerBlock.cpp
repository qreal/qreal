#include "speakerBlock.h"

#include <interpreterBase/blocksBase/common/displayBlock.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/twoD/parts/twoDSpeaker.h"
#include "src/robotModel/parts/trikSpeaker.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
: mRobotModel(robotModel)
{
}

SpeakerBlock::~SpeakerBlock()
{
}

void SpeakerBlock::run()
{
	QString const toSpeak = stringProperty("FileName");

	QString const port = "SpeakerPort";
	robotModel::parts::TrikSpeaker * const speaker
			= RobotModelUtils::findDevice<robotModel::parts::TrikSpeaker>(mRobotModel, port);

	if (speaker) {
		speaker->play(toSpeak);
	} else {
		error(tr("Speaker is not configured (WTF?)"));
	}

	emit done(mNextBlockId);
}
