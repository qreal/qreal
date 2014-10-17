#include "generatorBase/templateParametrizedEntity.h"

#include <QtCore/QDebug>

#include <qrutils/inFile.h>
#include <qrkernel/exception/exception.h>

using namespace generatorBase;
using namespace qReal;

TemplateParametrizedEntity::TemplateParametrizedEntity()
{
}

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
	QString result;

	try {
		result = utils::InFile::readAll(fullPath);
	} catch (qReal::Exception const &exception) {
		// Without this try-catch program would be failing every time when
		// someone forgets or missprints tamplate name or unknown block with
		// common generation rule will ty to read template
		qDebug() << "UNHANDLED EXCEPTION: " + exception.message();
	}

	return result;
}

void TemplateParametrizedEntity::setPathToTemplates(QString const &pathTemplates)
{
	mPathToRoot = pathTemplates;
}

QString TemplateParametrizedEntity::pathToRoot() const
{
	return mPathToRoot;
}
