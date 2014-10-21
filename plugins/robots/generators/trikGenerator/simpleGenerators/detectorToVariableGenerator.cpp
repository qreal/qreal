#include "detectorToVariableGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DetectorToVariableGenerator::DetectorToVariableGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t"
			, { Binding::createStaticConverting("@@BODY@@"
					, repo.property(id, "Variable").toString() + "="
								+ lineSensorVariableFromMode(repo.property(id, "Mode").toString())
					, customizer.factory()->functionBlockConverter(id))
			}, parent)
{
}

QString DetectorToVariableGenerator::lineSensorVariableFromMode(QString const &mode) const
{
	if (mode == "line") {
		return "lineSensorX";
	}

	if (mode == "line") {
		return "lineSensorX";
	}

	if (mode == "colorSensor") {
		return "colorSensorR";
	}

	/// @todo: Report an error?
	return "0";
}
