#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "trikGeneratorBase/trikGeneratorBaseDeclSpec.h"

namespace trik {

/// A base class for every generator from the TRIK kit.
class ROBOTS_TRIK_GENERATOR_BASE_EXPORT TrikGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikGeneratorPluginBase();

	QString kitId() const override;

protected:
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;
};

}
