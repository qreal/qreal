#include "inequalitySignConverter.h"

using namespace qReal::robots::generators::converters;

InequalitySignConverter::InequalitySignConverter(QString const &pathToTemplates)
	: TemplateParametrizedConverter(pathToTemplates)
{
}

QString InequalitySignConverter::convert(QString const &data) const
{
	return readTemplate(QString("signs/%1.t").arg(data));
}
