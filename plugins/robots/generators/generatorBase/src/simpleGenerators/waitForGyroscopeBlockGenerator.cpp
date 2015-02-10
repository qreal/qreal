#include "waitForGyroscopeBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForGyroscopeBlockGenerator::WaitForGyroscopeBlockGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/gyroscope.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->portNameConverter())
			<< Binding::createConverting("@@DEGREES@@", "Degrees"
					, customizer.factory()->intPropertyConverter(id, "Degrees"))
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}
