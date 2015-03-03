#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

AbstractSimpleGenerator::AbstractSimpleGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
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
