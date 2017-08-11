/* Copyright 2017 QReal Research Group
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

#include <generatorBase/masterGeneratorBase.h>

#include <QtCore/QScopedPointer>

namespace pioneer {
namespace lua {

class PioneerStateMachineGenerator;
class GotoLabelManager;

/// Main generator that directs generation process (mainly by configuring its base class that does actual job).
class PioneerLuaMasterGenerator : public generatorBase::MasterGeneratorBase
{
	Q_OBJECT

public:
	PioneerLuaMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

	~PioneerLuaMasterGenerator() override;

private:
	void initialize() override;
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void beforeGeneration() override;
	QString generate(const QString &indentString) override;

	/// Name of the generator, needed to locate templates (used as a prefix in path).
	const QString mGeneratorName;

	/// Control flow generator, defines strategy of traversing nodes and converting them into semantic tree, thus
	/// defining control flow in an application.
	QScopedPointer<PioneerStateMachineGenerator> mControlFlowGenerator;

	/// Storage and generator for human-readable goto labels.
	QScopedPointer<GotoLabelManager> mGotoLabelManager;
};

}
}
