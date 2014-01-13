#include "nxtKitInterpreterInterface.h"

using namespace nxtKitInterpreter;

QString NxtKitInterpreterInterface::kitId() const
{
	return "nxtKit";
}

QString NxtKitInterpreterInterface::friendlyKitName() const
{
	return tr("Empty Kit");
}

QWidget *NxtKitInterpreterInterface::settingsWidget() const
{
	return nullptr;
}

qReal::IdList NxtKitInterpreterInterface::specificBlocks() const
{
	return qReal::IdList();
}

qReal::IdList NxtKitInterpreterInterface::unsupportedBlocks() const
{
	return qReal::IdList();
}
