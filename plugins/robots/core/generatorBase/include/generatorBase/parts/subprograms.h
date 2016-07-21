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

#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {

class ControlFlowGeneratorBase;

namespace parts {

// TODO: make this class more customizable for concrete generators

/// Incapsulates operations used for subprograms processing
class ROBOTS_GENERATOR_EXPORT Subprograms : public TemplateParametrizedEntity
{
public:
	Subprograms(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const QStringList &pathsToTemplates
			, const simple::Binding::ConverterInterface *nameNormalizer);

	~Subprograms() override;

	/// Must be called each time when visitor has found subprogram call
	/// @param logicalId Logical id of the block which calls subprogram
	void usageFound(const qReal::Id &logicalId);

	/// Starts subprograms code generation process
	bool generate(ControlFlowGeneratorBase *mainGenerator, const QString &indentString);

	/// Returns the subprograms forward declarations code. If generation was unsuccessfull returns an empty string.
	QString forwardDeclarations() const;

	/// Returns the subprograms code. If generation was unsuccessfull returns an empty string.
	QString implementations() const;

	void appendManualSubprogram(const QString &name, const QString &body);

private:
	bool checkIdentifier(const QString &identifier, const QString &rawName);

	void obtainCode(QMap<qReal::Id, QString> const &declarations
			, QMap<qReal::Id, QString> const &implementations);

	QString generateManualDeclarations() const;

	// TODO: this must be obtained via models or smth
	qReal::Id graphicalId(const qReal::Id &logicalId) const;

	qReal::Id firstToGenerate() const;

	QString readSubprogramTemplate(const qReal::Id &id, const QString &pathToTemplate);

	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	const simple::Binding::ConverterInterface *mNameNormalizer;  // Takes ownership

	/// Stores all found by generator diagrams with subprograms implementation.
	/// Bool value means if key diagram was already processed and generated into
	/// the code.
	QMap<qReal::Id, bool> mDiscoveredSubprograms;

	QStringList mImplementationsCode;
	QStringList mForwardDeclarationsCode;

	QSet<QString> mUsedNames;

	QMap<QString, QString> mManualDeclarations;
};

}
}
