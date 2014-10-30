#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"

namespace nxt {

/// A base class for every generator from the NXT kit.
class ROBOTS_NXT_GENERATOR_BASE_EXPORT NxtGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	QString kitId() const override;
};

}
