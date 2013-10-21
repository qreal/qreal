#include "commentElementGenerator.h"

using namespace qReal;
using namespace robots::generators::simple;

CommentElementGenerator::CommentElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "comment.t", QList<Binding *>(), parent)
{
}
