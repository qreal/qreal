#include "colorConverter.h"

using namespace qReal::robots::generators::converters;

ColorConverter::ColorConverter(QString const &pathToTemplates)
	: TemplateParametrizedConverter(pathToTemplates)
{
}

QString ColorConverter::convert(QString const &data) const
{
	return readTemplate(QString("colors/%1.t").arg(data));
}
