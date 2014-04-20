#include "commentElementGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

CommentElementGenerator::CommentElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "comment.t", QList<Binding *>(), parent)
{
}
