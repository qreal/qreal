#include "trikGeneratorFactory.h"
#include "converters/trikEnginePortsConverter.h"
#include "simpleGenerators/trikEnginesGenerator.h"
#include "simpleGenerators/trikEnginesStopGenerator.h"

using namespace qReal::robots::generators;
using namespace simple;
using namespace trik;

TrikGeneratorFactory::TrikGeneratorFactory(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: GeneratorFactoryBase(repo, errorReporter)
{
}

TrikGeneratorFactory::~TrikGeneratorFactory()
{
}

AbstractSimpleGenerator *TrikGeneratorFactory::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType == "EnginesForward" || elementType == "EnginesBackward") {
		return new TrikEnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType == "EnginesStop") {
		return new TrikEnginesStopGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString TrikGeneratorFactory::pathToTemplates() const
{
	return ":/trik/templates";
}

Binding::MultiConverterInterface *TrikGeneratorFactory::enginesConverter() const
{
	return new converters::TrikEnginePortsConverter(pathToTemplates());
}

Binding::ConverterInterface *TrikGeneratorFactory::inputPortConverter() const
{
	return new Binding::EmptyConverter;
}

Binding::ConverterInterface *TrikGeneratorFactory::outputPortConverter() const
{
	return new Binding::EmptyConverter;
}
