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

#include "qrmcLauncher.h"
#include "pluginCompiler.h"
#include "pluginLoader.h"
#include "metamodelXmlGeneratorLauncher.h"
#include "htmlMaker.h"
#include "configurationFileParser.h"
#include "methodsTester.h"

#include <qrrepo/repoApi.h>
#include <qrgui/plugins/pluginManager/interpreterEditorManager.h>
#include <metaMetaModel/metamodel.h>

#include <QtCore/QString>

namespace editorPluginTestingFramework {

/// Class that directs testing process.
class MainClass
{
public:
	/// Gets name of .qrs file with metamodel and path to qrmc, launches all processes for testing.
	MainClass(
			const QString &metamodelFileName
			, const QString &pathToQrmc
			, const QString &applicationPath
			, const QString &configurationFileName
		);

	/// Returns result of comparison - 0, if all results are correct, and 1 otherwise.
	int testResult() const;

private:
	/// Returns normalized name (without extension).
	static QString normalizedName(const QString &fileName);

	/// Deletes old qrtest/binaries and all inner folders and files.
	static void deleteOldFiles(const QString &directory);

	/// Sets value of "temp" variable, which contains path to temporary folder with unsaved file.
	void setTempFolderValue();

	/// returns old value
	void restoreOldValueOfTempFolder() const;

	void launchQrmc(const QString &fileName);

	QFileInfo compilePlugin(const QString &fileName, PluginCompiler::MetamodelCompiler metamodelCompiler);

	void launchEditorGenerator(const QString &fileName, const QString &qrxcGeneratedBinariesDir);

	void createHtml(QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
			, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
			, QList<MethodsTester::ResultOfGenerating> timeResult
			, QList<MethodsTester::ResultOfGenerating> timeResultInterpter);

	/// puts names of generated plugins into mQrxcGeneratedPluginList (for further creation of EditorManager)
//	void appendPluginNames();

	/// Loads plugin and returns pointer to its interface.
	/// Transfers ownership to a caller.
	qReal::Metamodel* loadPlugin(const QFileInfo &file);

	PluginLoader mPluginLoader;

	QStringList mQrxcGeneratedPluginsList;

	HtmlMaker mHtmlMaker;

	ConfigurationFileParser mConfiguration;

	/// result of testing for travis
	int mResultOfTesting;

	/// old value of "temp" variable
	QString mTempOldValue;

	/// path to QReal sources
	QString mApplicationPath;
};

}
