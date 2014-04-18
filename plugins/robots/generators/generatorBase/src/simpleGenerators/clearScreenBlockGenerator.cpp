#include "clearScreenBlockGenerator.h"

#include "generatorBase/generatorCustomizer.h"
#include "src/parts/images.h"

using namespace qReal::robots::generators::simple;

ClearScreenBlockGenerator::ClearScreenBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, customizer.factory()->images()->bmpFilesCount()
					// WARNING: this may not work when we traverse here earlier than
					// into drawing block but it will be executed before this block
					? "drawing/clearUsedScreen.t"
					: "drawing/clearUnusedScreen.t"
			, QList<Binding *>(), parent)
{
}
