#include "waitForTouchSensorBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForTouchSensorBlockGenerator::WaitForTouchSensorBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/touch.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}
