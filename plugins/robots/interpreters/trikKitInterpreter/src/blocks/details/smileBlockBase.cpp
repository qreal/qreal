#include "smileBlockBase.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

SmileBlockBase::SmileBlockBase(QString const &pathToImage, robotModel::parts::TrikDisplay &display)
	: mSmile(pathToImage)
	, mDisplay(display)
{
}

void SmileBlockBase::run()
{
	/// @todo: restore this.
	//	mDisplay.drawImage(mSmile);
	emit done(mNextBlockId);
}
