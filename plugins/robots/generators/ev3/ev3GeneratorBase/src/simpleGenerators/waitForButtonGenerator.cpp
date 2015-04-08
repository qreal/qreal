#include "waitForButtonGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/ev3GeneratorCustomizer.h"

using namespace ev3::simple;
using namespace generatorBase::simple;

WaitForButtonGenerator::WaitForButtonGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &buttonTemplate
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, buttonTemplate, QList<Binding *>()
			<< Binding::createStaticConverting("@@Block@@", "Block"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->goToBlockNumberConverter())
			, parent)
{
}

