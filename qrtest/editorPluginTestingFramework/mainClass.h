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
#include "qrxcLauncher.h"
#include "htmlMaker.h"
#include "configurationFileParser.h"
#include "methodsTester.h"

#include <qrrepo/repoApi.h>
#include <qrgui/plugins/pluginManager/interpreterEditorManager.h>
#include <qrgui/plugins/metaMetaModel/metamodel.h>

#include <QtCore/QString>

namespace editorPluginTestingFramework {

class MainClass
{
public:
	/// gets name of qrs-file and path to qrmc, launches all processes for testing
	MainClass(
			const QString &fileName
			, const QString &pathToQrmc
			, const QString &applicationPath
			, const QString &configurationFileName
		);

	/// returns result of comparison - 0, if all results are correct, and 1 otherwise
	int travisTestResult() const;

private:
	/// returns normalized name (without extension)
	static QString normalizedName(const QString &fileName);

	/// deletes old qrtest/binaries and all inner folders and files
	static void deleteOldBinaries(const QString &directory);

	/// creates one folder
	void createFolder(const QString &path);

	/// creates qrtest/binaries and all inner folders
	void createNewFolders();

	/// copies testMetamodel.qrs from qrtest/editorPluginTestingFramework/fileToTestWithTravis to bin
	static void copyTestMetamodel(const QString &fileName);

	/// sets value of tamp variable
	void setTempValueInSettingsManager();

	/// returns old value
	void returnOldValueOfTemp() const;

	void launchQrmc(const QString &fileName, const QString &pathToQrmc);

	void compilePlugin(const QString &directoryToCodeToCompile, const QString &fileName);

	void launchQrxc(const QString &fileName);

	void createHtml(QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
			, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
			, QList<MethodsTester::ResultOfGenerating> timeResult
			, QList<MethodsTester::ResultOfGenerating> timeResultInterpter);

	/// puts names of generated plugins into mQrxcGeneratedPluginList (for further creation of EditorManager)
	void appendPluginNames();

	/// parses file with configuration settings
	void parseConfigurationFile(const QString &fileName);

	/// returns loaded editorInterface
	qReal::Metamodel* loadedPlugin(const QString &fileName, const QString &pathToFile);

	QrmcLauncher mQrmcLauncher;

	PluginCompiler mPluginCompiler;

	PluginLoader mPluginLoader;

	QrxcLauncher mQrxcLauncher;

	QStringList mQrxcGeneratedPluginsList;

	HtmlMaker mHtmlMaker;

	ConfigurationFileParser mConfigurationFileParser;

	/// path to qmake
	QString mQmakeParameter;

	/// path to make
	QString mMakeParameter;

	/// configuration: debug or release
	QString mConfigurationParameter;

	/// plugin extension (.dll for windows, .so for linux, etc.)
	QString mPluginExtension;

	/// prefix (none for Windows, lib for linux)
	QString mPrefix;

	/// path to qreal root (from bin)
	QString mQRealRootPath;

	/// "yes" if we want to generate html and "no" otherwise
	QString mGenerateHtml;

	/// path to generated code (from root)
	QString mGeneratedCodeDirQrxc;
	QString mGeneratedCodeDirQrmc;
	QString mGeneratedDirHtml;

	/// result of testing for travis
	int mResultOfTesting;

	/// old value of "temp" variable
	QString mTempOldValue;

	/// path to QReal sources
	QString mApplicationPath;
};

}

