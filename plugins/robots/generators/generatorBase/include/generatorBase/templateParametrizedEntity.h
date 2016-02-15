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

#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "robotsGeneratorDeclSpec.h"

namespace generatorBase {

/// This class can be inherited by those entities who need to use generator templates
class ROBOTS_GENERATOR_EXPORT TemplateParametrizedEntity
{
public:
	TemplateParametrizedEntity();

	/// @param pathToTemplates A path to a folder containing all concrete generator templates
	explicit TemplateParametrizedEntity(const QStringList &pathsToTemplates);

	virtual ~TemplateParametrizedEntity();

	/// Resets a path to a folder containing all concrete generator templates
	void setPathsToTemplates(const QStringList &pathsTemplates);

protected:
	/// Reads the given file contents. A path to file must be relative to templates folder root.
	/// @param pathFromRoot A path to a concrete template relatively to specified in constructor folder.
	QString readTemplate(const QString &pathFromRoot) const;

	/// Reads the given file contents if such exists or returns @arg fallback string otherwise.
	/// Same as readTemplate() but does not display 'file not found' messages in debug output.
	/// A path to file must be relative to templates folder root.
	/// @param pathFromRoot A path to a concrete template relatively to specified in constructor folder.
	QString readTemplateIfExists(const QString &pathFromRoot, const QString &fallback = QString()) const;

	/// Returns a given in constructor path to tempates root.
	QStringList pathsToRoot() const;

	/// Replaces @@RANDOM_ID@@ occurences to some random c++ identifier.
	/// Do not make \a templateString const&, replace() will be called on it.
	QString addRandomIds(QString templateString) const;

private:
	QStringList mPathsToRoot;
};

}
