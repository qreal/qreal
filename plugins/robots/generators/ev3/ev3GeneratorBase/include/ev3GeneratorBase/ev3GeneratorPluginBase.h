#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"

namespace ev3 {
class ROBOTS_EV3_GENERATOR_BASE_EXPORT Ev3GeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	Ev3GeneratorPluginBase();

	QString kitId() const override;
};
}
