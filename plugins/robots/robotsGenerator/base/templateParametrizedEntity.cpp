#include "templateParametrizedEntity.h"

#include <inFile.h>

using namespace qReal::robots::generators;

TemplateParametrizedEntity::TemplateParametrizedEntity(QString const &pathToTemplates)
	: mPathToRoot(pathToTemplates)
{
}

TemplateParametrizedEntity::~TemplateParametrizedEntity()
{
}

QString TemplateParametrizedEntity::readTemplate(QString const &pathFromRoot) const
{
	QString const fullPath = mPathToRoot + '/' + pathFromRoot;
	return utils::InFile::readAll(fullPath);
}
