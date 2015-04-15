/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "generatorBase/templateParametrizedEntity.h"

#include <QtCore/QDebug>

#include <qrutils/inFile.h>
#include <qrkernel/exception/exception.h>

using namespace generatorBase;

TemplateParametrizedEntity::TemplateParametrizedEntity()
{
}

TemplateParametrizedEntity::TemplateParametrizedEntity(const QString &pathToTemplates)
	: mPathToRoot(pathToTemplates)
{
}

TemplateParametrizedEntity::~TemplateParametrizedEntity()
{
}

QString TemplateParametrizedEntity::readTemplate(const QString &pathFromRoot) const
{
	const QString fullPath = mPathToRoot + '/' + pathFromRoot;
	QString result;

	try {
		result = utils::InFile::readAll(fullPath);
	} catch (const qReal::Exception &exception) {
		// Without this try-catch program would be failing every time when
		// someone forgets or missprints tamplate name or unknown block with
		// common generation rule will ty to read template
		qDebug() << "UNHANDLED EXCEPTION: " + exception.message();
	}

	return result;
}

QString TemplateParametrizedEntity::readTemplateIfExists(const QString &pathFromRoot, const QString &fallback) const
{
	const QString fullPath = mPathToRoot + '/' + pathFromRoot;
	if (!QFile(fullPath).exists()) {
		return fallback;
	}

	QString result = fallback;

	try {
		result = utils::InFile::readAll(fullPath);
	} catch (const qReal::Exception &) {
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

void TemplateParametrizedEntity::setPathToTemplates(const QString &pathTemplates)
{
	mPathToRoot = pathTemplates;
}
