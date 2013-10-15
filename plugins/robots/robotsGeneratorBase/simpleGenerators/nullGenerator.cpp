#include "nullGenerator.h"

using namespace qReal::robots::generators::simple;

NullGenerator::NullGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
{
}

QString NullGenerator::generate()
{
	return QString();
}
