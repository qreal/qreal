#include "trikEnginePortsConverter.h"

using namespace qReal::robots::generators::converters;

TrikEnginePortsConverter::TrikEnginePortsConverter(QString const &pathToTemplates)
	: TemplateParametrizedMultiConverter(pathToTemplates)
{
}

QStringList TrikEnginePortsConverter::convert(QString const &data) const
{
	return data.toUpper().split(QRegExp("[^\\d]"), QString::SkipEmptyParts);
}
