#include "waitForMotionGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForMotionGenerator::WaitForMotionGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/motion.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->portNameConverter())
			<< Binding::createConverting("@@DISTANCE@@", "Distance"
					, customizer.factory()->intPropertyConverter(id, "Distance"))
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}
