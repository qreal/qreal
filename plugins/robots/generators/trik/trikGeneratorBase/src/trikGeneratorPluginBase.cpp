#include "trikGeneratorBase/trikGeneratorPluginBase.h"

using namespace trik;

TrikGeneratorPluginBase::TrikGeneratorPluginBase()
{
}

QString TrikGeneratorPluginBase::kitId() const
{
	return "trikKit";
}

void TrikGeneratorPluginBase::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
