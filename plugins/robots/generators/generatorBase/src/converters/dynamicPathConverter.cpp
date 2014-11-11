#include "generatorBase/converters/dynamicPathConverter.h"

using namespace generatorBase::converters;

DynamicPathConverter::DynamicPathConverter(QString const &pathToTemplates, QString const &pathFromRoot)
	: TemplateParametrizedConverter(pathToTemplates)
	, mPathFromRoot(pathFromRoot)
{
}

QString DynamicPathConverter::convert(QString const &data) const
{
	QString mutablePath = mPathFromRoot;
	return readTemplate(mutablePath.replace("@@DATA@@", data));
}
