#include "detectorToVariableGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DetectorToVariableGenerator::DetectorToVariableGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t"
			, { Binding::createStaticConverting("@@BODY@@"
					, repo.property(id, "Variable").toString().isEmpty()
							// For more comprehensible error reporting we pass an empty string if there is empty
							// property in a repository, otherwise parser will be complaining about "= lineSensorX",
							// but that string is not even seen by user.
							? ""
							: repo.property(id, "Variable").toString() + "= lineSensorX"
					, customizer.factory()->functionBlockConverter(id, "Variable"))
			}, parent)
{
}
