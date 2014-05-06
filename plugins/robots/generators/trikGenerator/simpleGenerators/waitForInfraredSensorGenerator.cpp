#include "waitForInfraredSensorGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

int const sleepInterval = 10;

WaitForInfraredSensorGenerator::WaitForInfraredSensorGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/infrared.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@DISTANCE@@", "Distance", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}
