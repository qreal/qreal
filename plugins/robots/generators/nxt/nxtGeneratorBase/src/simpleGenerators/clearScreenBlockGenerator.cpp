#include "clearScreenBlockGenerator.h"

#include "src/nxtGeneratorCustomizer.h"
#include "nxtGeneratorBase/parts/images.h"

using namespace nxt::simple;

ClearScreenBlockGenerator::ClearScreenBlockGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, static_cast<NxtGeneratorFactory *>(customizer.factory())->images().bmpFilesCount()
					// WARNING: this may not work when we traverse here earlier than
					// into drawing block but it will be executed before this block
					? "drawing/clearUsedScreen.t"
					: "drawing/clearUnusedScreen.t"
			, {}, parent)
{
}
