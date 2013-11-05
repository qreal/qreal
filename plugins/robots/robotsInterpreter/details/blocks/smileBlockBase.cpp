#include "smileBlockBase.h"

using namespace qReal::interpreters::robots::details::blocks;

SmileBlockBase::SmileBlockBase(QString const &pathToImage, robotParts::Display &display)
	: mSmile(pathToImage)
	, mDisplay(display)
{
}

void SmileBlockBase::run()
{
	mDisplay.drawImage(mSmile);
	emit done(mNextBlock);
}
