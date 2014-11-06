#include "generatorBase/templateParametrizedEntity.h"

#include <QtCore/QDebug>

#include <qrutils/inFile.h>
#include <qrkernel/exception/exception.h>

using namespace generatorBase;

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

QString TemplateParametrizedEntity::readTemplateIfExists(QString const &pathFromRoot, QString const &fallback) const
{
	QString const fullPath = mPathToRoot + '/' + pathFromRoot;
	if (!QFile(fullPath).exists()) {
		return fallback;
	}

	QString result = fallback;

	try {
		result = utils::InFile::readAll(fullPath);
	} catch (qReal::Exception const &) {
		// Without this try-catch program would be failing every time when
		// someone forgets or missprints tamplate name or unknown block with
		// common generation rule will ty to read template
	}

	return result;
}

QString TemplateParametrizedEntity::pathToRoot() const
{
	return mPathToRoot;
}

void TemplateParametrizedEntity::setPathToTemplates(QString const &pathTemplates)
{
	mPathToRoot = pathTemplates;
}
