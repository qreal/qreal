/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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
#include <qrkernel/logging.h>

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
	QString errorMessage;
	const QString result = utils::InFile::readAll(fullPath, &errorMessage);
	if (!errorMessage.isEmpty()) {
		QLOG_ERROR() << "Reading from template while generating code failed";
		qWarning() << errorMessage;
	}

	return result;
}

QString TemplateParametrizedEntity::readTemplateIfExists(const QString &pathFromRoot, const QString &fallback) const
{
	const QString fullPath = mPathToRoot + '/' + pathFromRoot;
	QString errorMessage;
	const QString result = utils::InFile::readAll(fullPath, &errorMessage);
	if (!errorMessage.isEmpty()) {
		return fallback;
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
