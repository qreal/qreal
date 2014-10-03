#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

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
