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
#include <QtCore/QUuid>

#include <qrutils/inFile.h>
#include <qrutils/nameNormalizer.h>
#include <qrkernel/logging.h>

using namespace generatorBase;

TemplateParametrizedEntity::TemplateParametrizedEntity()
{
}

TemplateParametrizedEntity::TemplateParametrizedEntity(const QStringList &pathsToTemplates)
	: mPathsToRoot(pathsToTemplates)
{
}

TemplateParametrizedEntity::~TemplateParametrizedEntity()
{
}

QString TemplateParametrizedEntity::readTemplate(const QString &pathFromRoot) const
{
	for (const QString &path: mPathsToRoot) {
		const QString fullPath = path + '/' + pathFromRoot;
		if (QFile::exists(fullPath)) {
			QString errorMessage;
			const QString result = utils::InFile::readAll(fullPath, &errorMessage);
			if (!errorMessage.isEmpty()) {
				QLOG_ERROR() << "Reading from template while generating code failed";
				qWarning() << "TemplateParametrizedEntity::readTemplate" << errorMessage;
			}

			return result;
		}
	}

	QLOG_ERROR() << "Template" << pathFromRoot << "not found in" << mPathsToRoot;
	qWarning() << "Template" << pathFromRoot << "not found in" << mPathsToRoot;

	return "";
}

QString TemplateParametrizedEntity::readTemplateIfExists(const QString &pathFromRoot, const QString &fallback) const
{
	for (const QString &path: mPathsToRoot) {
		const QString fullPath = path + '/' + pathFromRoot;
		if (QFile::exists(fullPath)) {
			QString errorMessage;
			const QString result = utils::InFile::readAll(fullPath, &errorMessage);
			if (!errorMessage.isEmpty()) {
				QLOG_ERROR() << "Reading from template while generating code failed";
				qWarning() << "TemplateParametrizedEntity::readTemplate" << errorMessage;
			} else {
				return result;
			}
		}
	}

	return fallback;
}

QStringList TemplateParametrizedEntity::pathsToRoot() const
{
	return mPathsToRoot;
}

QString TemplateParametrizedEntity::addRandomIds(QString templateString) const
{
	int index = 0;
	QString randomId = utils::NameNormalizer::normalizeStrongly(QUuid::createUuid().toString(), false);
	templateString.replace("@@RANDOM_ID@@", randomId);
	while (true) {
		const QString pattern = QString("@@RANDOM_ID_%1@@").arg(++index);
		if (!templateString.contains(pattern)) {
			break;
		}

		randomId = utils::NameNormalizer::normalizeStrongly(QUuid::createUuid().toString(), false);
		templateString.replace(pattern, randomId);
	}

	return templateString;
}

void TemplateParametrizedEntity::setPathsToTemplates(const QStringList &pathsTemplates)
{
	mPathsToRoot = pathsTemplates;
}
