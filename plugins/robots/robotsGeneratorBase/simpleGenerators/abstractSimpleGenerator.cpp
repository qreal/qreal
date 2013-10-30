#include "abstractSimpleGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

AbstractSimpleGenerator::AbstractSimpleGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: QObject(parent)
	, TemplateParametrizedEntity(customizer.factory()->pathToTemplates())
	, mRepo(repo)
	, mCustomizer(customizer)
	, mId(id)
{
}

AbstractSimpleGenerator::~AbstractSimpleGenerator()
{
}
