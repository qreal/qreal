#include "goToBlockNumberConverter.h"

using namespace ev3::converters;

GoToBlockNumberConverter::GoToBlockNumberConverter(const QString &goToBlockNumber)
	:mGoToBlockNumber(goToBlockNumber)
{
}

QString GoToBlockNumberConverter::convert(const QString &data) const
{
	Q_UNUSED(data)
	return "Block" + mGoToBlockNumber;
}
