#include "systemGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

SystemGenerator::SystemGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, repo.property(id, "Code").toBool() ? "nativeCode.t" : "system.t"
			, { Binding::createConverting("@@COMMAND@@", "Command"
					, customizer.factory()->stringPropertyConverter(id, "Command"))
				}
			, parent)
{
}
