#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

namespace trik {

/// A base class for every generator from the TRIK kit.
class TrikGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikGeneratorPluginBase();

	QString kitId() const override;

protected:
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
};

}
