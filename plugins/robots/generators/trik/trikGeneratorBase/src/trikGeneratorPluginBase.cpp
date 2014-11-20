#include "trikGeneratorBase/trikGeneratorPluginBase.h"

using namespace trik;

TrikGeneratorPluginBase::TrikGeneratorPluginBase()
{
}

QString TrikGeneratorPluginBase::kitId() const
{
	return "trikKit";
}

void TrikGeneratorPluginBase::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
