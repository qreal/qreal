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

#include <QtCore/QObject>

#include <qrrepo/repoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "robotsGeneratorDeclSpec.h"
#include "generatorCustomizer.h"
#include "controlFlowGeneratorBase.h"
#include "templateParametrizedEntity.h"
#include "primaryControlFlowValidator.h"

class QFileInfo;

namespace utils {
class ParserErrorReporter;
}

namespace qrtext {
class LanguageToolboxInterface;
}

namespace generatorBase {

namespace lua {
class LuaProcessor;
}

class ReadableControlFlowGenerator;
class GotoControlFlowGenerator;

/// The main class for every code generator. Produces nessesary components and
/// controls their work. Must be reimplemented in each concrete generator.
class ROBOTS_GENERATOR_EXPORT MasterGeneratorBase : public QObject, public TemplateParametrizedEntity
{
	Q_OBJECT

public:
	MasterGeneratorBase(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const utils::ParserErrorReporter &parserErrorReporter
			, const qReal::Id &diagramId);

	void setProjectDir(const QFileInfo &fileInfo);

	/// @warning This method mustn`t be called from constructor. Otherwise
	/// there will be segfault due to pure virtual method call in constructor
	virtual void initialize();

	/// Starts code generation process. Returns path to file with generated code
	/// if it was successfull and an empty string otherwise.
	virtual QString generate(const QString &indentString);

protected:
	virtual GeneratorCustomizer *createCustomizer() = 0;

	/// Default implementation takes ownership via QObject parentship system.
	virtual lua::LuaProcessor *createLuaProcessor();

	/// Default implementation takes ownership via QObject parentship system.
	virtual PrimaryControlFlowValidator *createValidator();

	/// Implementation must return a path to a file where all generated code
	/// will be written. Called on the last stage of the generation process
	/// so concrete generators have time to 'prepare' this path
	virtual QString targetPath() = 0;

	virtual bool supportsGotoGeneration() const = 0;

	virtual void beforeGeneration();
	virtual void processGeneratedCode(QString &generatedCode);
	virtual void afterGeneration();

	void outputCode(const QString &path, const QString &code);

	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	qrtext::LanguageToolboxInterface &mTextLanguage;
	qReal::Id mDiagram;
	GeneratorCustomizer *mCustomizer;
	PrimaryControlFlowValidator *mValidator;
	ReadableControlFlowGenerator *mReadableControlFlowGenerator;  // Takes ownership
	GotoControlFlowGenerator *mGotoControlFlowGenerator;  // Takes ownership
	QString mProjectName;
	QString mProjectDir;
	int mCurInitialNodeNumber;
	const utils::ParserErrorReporter &mParserErrorReporter;
};

}
