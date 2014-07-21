#include "speakerBlock.h"

#include <QtGui/QColor>

#include <interpreterBase/blocksBase/common/displayBlock.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/twoD/parts/twoDSpeaker.h"
#include "src/robotModel/parts/trikSpeaker.h"

using namespace trikKitInterpreter;
using namespace blocks;
using namespace details;
using namespace interpreterBase::robotModel;
using namespace utils;

SpeakerBlock::SpeakerBlock(RobotModelInterface &robotModel)
: mRobotModel(robotModel)
{
}

SpeakerBlock::~SpeakerBlock()
{
}

void SpeakerBlock::run()
{

	QString speach = stringProperty("FileName");

	QString const port = "SpeakerPort";
	robotModel::parts::TrikSpeaker * const speaker = RobotModelUtils::findDevice<robotModel::parts::TrikSpeaker>(mRobotModel, port);
	if (speaker) {
		speaker->play(speach);
	} else {
		error(tr("Speaker is not configured (WTF?)"));
	}

	emit done(mNextBlockId);
}
