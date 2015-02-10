#include "waitForColorIntensityBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForColorIntensityBlockGenerator::WaitForColorIntensityBlockGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/colorIntensity.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->portNameConverter())
			<< Binding::createConverting("@@INTENSITY@@", "Intensity"
					, customizer.factory()->intPropertyConverter(id, "Intensity"))
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}
