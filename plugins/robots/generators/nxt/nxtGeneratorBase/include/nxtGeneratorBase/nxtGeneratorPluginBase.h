#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

namespace nxt {

/// A base class for every generator from the NXT kit.
class NxtGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	QString kitId() const override;
};

}
