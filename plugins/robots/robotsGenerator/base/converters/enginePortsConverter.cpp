#include "enginePortsConverter.h"

using namespace qReal::robots::generators::converters;

EnginePortsConverter::EnginePortsConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *oneEngineConverter)
	: TemplateParametrizedMultiConverter(pathToTemplates)
	, mOneEngineConverter(oneEngineConverter)
{
}

EnginePortsConverter::~EnginePortsConverter()
{
	delete mOneEngineConverter;
}

QStringList EnginePortsConverter::convert(QString const &data) const
{
	QStringList const ports = data.toUpper().split(QRegExp("[^ABC]"), QString::SkipEmptyParts);
	QStringList result;
	foreach (QString const &port, ports) {
		result << mOneEngineConverter->convert(port);
	}

	return result;
}
