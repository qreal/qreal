#include "waitForColorIntensityBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForColorIntensityBlockGenerator::WaitForColorIntensityBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/colorIntensity.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@INTENSITY@@", "Intensity", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}
