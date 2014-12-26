#include "waitForColorBlockGenerator.h"

#include "src/nxtGeneratorCustomizer.h"

using namespace nxt::simple;
using namespace generatorBase::simple;

WaitForColorBlockGenerator::WaitForColorBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/color.t", {
			Binding::createConverting("@@PORT@@", "Port", customizer.factory()->portNameConverter())
			, Binding::createConverting("@@COLOR@@", "Color"
					, static_cast<NxtGeneratorFactory *>(customizer.factory())->colorConverter())
			}, parent)
{
}
