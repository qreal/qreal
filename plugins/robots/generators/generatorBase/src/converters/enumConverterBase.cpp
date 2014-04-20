#include "enumConverterBase.h"

using namespace generatorBase::converters;
using namespace qReal;

EnumConverterBase::EnumConverterBase(QString const &pathToTemplatesDir
		, QMap<QString, QString> const &valuesToTemplatePaths)
	: TemplateParametrizedConverter(pathToTemplatesDir)
	, mValuesToTemplates(valuesToTemplatePaths)
{
}

void EnumConverterBase::addMapping(QString const &enumValue, QString const &templatePath)
{
	mValuesToTemplates[enumValue] = templatePath;
}

QString EnumConverterBase::convert(QString const &data) const
{
	foreach (QString const &value, mValuesToTemplates.keys()) {
		if (value == data) {
			return readTemplate(mValuesToTemplates[value]);
		}
	}

	return QString();
}
