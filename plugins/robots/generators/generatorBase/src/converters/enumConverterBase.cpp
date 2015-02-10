#include "generatorBase/converters/enumConverterBase.h"

using namespace generatorBase::converters;
using namespace qReal;

EnumConverterBase::EnumConverterBase(const QString &pathToTemplatesDir
		, QMap<QString, QString> const &valuesToTemplatePaths)
	: TemplateParametrizedConverter(pathToTemplatesDir)
	, mValuesToTemplates(valuesToTemplatePaths)
{
}

void EnumConverterBase::addMapping(const QString &enumValue, const QString &templatePath)
{
	mValuesToTemplates[enumValue] = templatePath;
}

QString EnumConverterBase::convert(const QString &data) const
{
	foreach (const QString &value, mValuesToTemplates.keys()) {
		if (value == data) {
			return readTemplate(mValuesToTemplates[value]);
		}
	}

	return QString();
}
