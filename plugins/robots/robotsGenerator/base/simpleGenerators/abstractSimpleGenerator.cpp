#include "abstractSimpleGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

AbstractSimpleGenerator::AbstractSimpleGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: QObject(parent)
	, TemplateParametrizedEntity(customizer.factory()->pathToTemplates())
	, mModel(model)
	, mCustomizer(customizer)
	, mId(id)
{
}

AbstractSimpleGenerator::~AbstractSimpleGenerator()
{
}
