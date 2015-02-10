#include "nullGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

NullGenerator::NullGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
{
}

QString NullGenerator::generate()
{
	return QString();
}
