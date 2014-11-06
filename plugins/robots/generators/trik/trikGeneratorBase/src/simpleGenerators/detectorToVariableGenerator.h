#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for "line detector reading to variable" block.
class DetectorToVariableGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DetectorToVariableGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);

private:
	QString lineSensorVariableFromMode(QString const &mode) const;
};

}
}
