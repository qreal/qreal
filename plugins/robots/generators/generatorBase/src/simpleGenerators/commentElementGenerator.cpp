#include "commentElementGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

CommentElementGenerator::CommentElementGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "comment.t", QList<Binding *>(), parent)
{
}
