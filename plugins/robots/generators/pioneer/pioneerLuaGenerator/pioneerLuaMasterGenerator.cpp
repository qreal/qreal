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

#include "pioneerLuaMasterGenerator.h"

#include <QtCore/QDir>

#include <qrtext/languageToolboxInterface.h>
#include <qrutils/stringUtils.h>
#include <generatorBase/parts/initTerminateCodeGenerator.h>
#include <generatorBase/gotoControlFlowGenerator.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/threads.h>
#include <generatorBase/parts/variables.h>

#include "pioneerLuaGeneratorCustomizer.h"

using namespace pioneer::lua;
using namespace generatorBase;

using namespace qReal;

PioneerLuaMasterGenerator::PioneerLuaMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, parserErrorReporter, diagramId)
	, mGeneratorName(generatorName)
{
}

generatorBase::GeneratorCustomizer *PioneerLuaMasterGenerator::createCustomizer()
{
	return new PioneerLuaGeneratorCustomizer(
			mRepo
			, mErrorReporter
			, mRobotModelManager
			, *createLuaProcessor()
			, mGeneratorName);
}

QString PioneerLuaMasterGenerator::targetPath()
{
	return QString("%1/%2.lua").arg(mProjectDir, mProjectName);
}

bool PioneerLuaMasterGenerator::supportsGotoGeneration() const
{
	return true;
}

QString PioneerLuaMasterGenerator::generate(const QString &indentString)
{
	QDir().mkpath(mProjectDir + "/ap/");
	QDir().mkpath(mProjectDir + "/Ev/");
	QFile::copy(":/pioneer/lua/templates/testStub/ap/lua.lua", mProjectDir + "/ap/lua.lua");
	QFile::copy(":/pioneer/lua/templates/testStub/Ev/lua.lua", mProjectDir + "/Ev/lua.lua");
	return MasterGeneratorBase::generate(indentString);
}
