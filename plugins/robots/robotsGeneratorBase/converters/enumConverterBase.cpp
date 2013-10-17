#include "enumConverterBase.h"

using namespace qReal::robots::generators::converters;

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
