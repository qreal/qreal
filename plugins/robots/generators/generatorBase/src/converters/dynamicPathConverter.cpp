#include "generatorBase/converters/dynamicPathConverter.h"

using namespace generatorBase::converters;

DynamicPathConverter::DynamicPathConverter(const QString &pathToTemplates, const QString &pathFromRoot)
	: TemplateParametrizedConverter(pathToTemplates)
	, mPathFromRoot(pathFromRoot)
{
}

QString DynamicPathConverter::convert(const QString &data) const
{
	QString mutablePath = mPathFromRoot;
	return readTemplate(mutablePath.replace("@@DATA@@", data));
}
