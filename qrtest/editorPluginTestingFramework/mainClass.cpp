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

#include "mainClass.h"
#include "defs.h"
#include "methodsTesterForQrxcAndQrmc.h"
#include "methodsTesterForQrxcAndInterpreter.h"
#include "methodsCheckerForTravis.h"

#include "qrgui/plugins/pluginManager/interpreterEditorManager.h"
#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/plugins/pluginManager/editorManager.h"
#include "qrutils/xmlUtils.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

using namespace qReal;
using namespace editorPluginTestingFramework;
using namespace qrRepo;

MainClass::MainClass(
		const QString &fileName
		, const QString &pathToQrmc
		, const QString &applicationPath
		, const QString &configurationFileName)
		: mTempOldValue(SettingsManager::value("temp").toString())
		, mApplicationPath(applicationPath)
{
	setTempValueInSettingsManager();

	qDebug() << "configuration file: " << configurationFileName;
	parseConfigurationFile(configurationFileName);

	deleteOldBinaries(mGeneratedCodeDirQrxc);
	deleteOldBinaries(mGeneratedCodeDirQrmc);

	createNewFolders();
	const QString normalizedFileName = normalizedName(fileName);

	copyTestMetamodel(fileName);

	launchQrxc(normalizedFileName);
	compilePlugin(mGeneratedCodeDirQrxc + pathToQrxcGeneratedCode, normalizedFileName);
	Metamodel* const qrxcGeneratedPlugin = loadedPlugin(normalizedFileName, mGeneratedCodeDirQrxc + pathToQrxcGeneratedPlugin);
	appendPluginNames();

	launchQrmc(fileName, pathToQrmc);
	compilePlugin(mGeneratedCodeDirQrmc + pathToQrmcGeneratedCode, normalizedFileName);
	Metamodel* const qrmcGeneratedPlugin = loadedPlugin(normalizedFileName, mGeneratedCodeDirQrmc + pathToQrmcGeneratedPlugin);

	InterpreterEditorManager interpreterEditorManager(fileName, nullptr);
	QString path = "plugins/editors/qrtest/qrxc/plugins";
	EditorManager qrxcEditorManager(path);
	// we cast qrxc plugin to Editor Manager

	MethodsTesterForQrxcAndInterpreter* const interpreterMethodsTester = new MethodsTesterForQrxcAndInterpreter(
			&qrxcEditorManager
			,&interpreterEditorManager);

	QList<MethodsTester::ResultOfGenerating> interpreterMethodsTesterOutput =
			interpreterMethodsTester->generatedResult();
	QList<MethodsTester::ResultOfGenerating> methodsTimeOutputInterpreter = interpreterMethodsTester->generateTimeResult();


	if ((qrxcGeneratedPlugin != NULL) && (qrmcGeneratedPlugin != NULL)) {
		MethodsTesterForQrxcAndQrmc* const methodsTester = new MethodsTesterForQrxcAndQrmc(
				qrmcGeneratedPlugin, qrxcGeneratedPlugin);

		QList<MethodsTester::ResultOfGenerating> methodsTesterOutput = methodsTester->generatedOutput();
		QList<MethodsTester::ResultOfGenerating> methodsTimeOutput = methodsTester->generateTimeResult();

		if (mGenerateHtml == "yes") {
			createHtml(methodsTesterOutput, interpreterMethodsTesterOutput, methodsTimeOutput, methodsTimeOutputInterpreter);
		}

		mResultOfTesting = MethodsCheckerForTravis::calculateResult(methodsTesterOutput
				, interpreterMethodsTesterOutput);
	} else {
		qDebug() << "Generation of plugins failed";
	}

	returnOldValueOfTemp();
}

int MainClass::travisTestResult() const
{
	return mResultOfTesting;
}

void MainClass::createFolder(const QString &path)
{
	QDir dir;
	if (!dir.exists(path)) {
		dir.mkdir(path);
	}
}

void MainClass::createNewFolders()
{
	createFolder(mGeneratedCodeDirQrxc);
	createFolder(mGeneratedCodeDirQrxc + pluginsDir);
	createFolder(mGeneratedCodeDirQrxc + sourcesDir);
	createFolder(mGeneratedCodeDirQrmc + pathToQrmcGeneratedPlugin);
	createFolder(mGeneratedCodeDirQrxc + pathToQrxcGeneratedPlugin);
}

QString MainClass::normalizedName(const QString &fileName)
{
	QString normalizedName = fileName;
	if (fileName.contains("/")) {
		QStringList splittedName = normalizedName.split("/");
		normalizedName = splittedName.last();
	}

	if (normalizedName.contains(".qrs")) {
		normalizedName.chop(4);
	}

	return normalizedName;
}

void MainClass::deleteOldBinaries(const QString &directory)
{
	QDir dir(directory);
	if (!dir.exists()) {
		return;
	}

	for (const QFileInfo &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			deleteOldBinaries(fileInfo.filePath());
			dir.rmdir(fileInfo.fileName());
		} else {
			dir.remove(fileInfo.fileName());
		}
	}
}

void MainClass::copyTestMetamodel(const QString &fileName)
{
	const QString workingDirName = pathToTestMetamodel;
	QDir sourceDir(workingDirName);

	const QFileInfo destDirInfo;
	QDir destDir = destDirInfo.dir();

	QFile::copy(sourceDir.absolutePath() + "/" + fileName, destDir.absolutePath() + "/" + fileName);
}

void MainClass::setTempValueInSettingsManager()
{
	SettingsManager::setValue("temp", mApplicationPath + tempValueForSettingsManager);
}

void MainClass::returnOldValueOfTemp() const
{
	SettingsManager::setValue("temp", mTempOldValue);
}

void MainClass::launchQrmc(const QString &fileName, const QString &pathToQrmc)
{
	mQrmcLauncher.launchQrmc(fileName, pathToQrmc, mGeneratedCodeDirQrmc);
}

void MainClass::compilePlugin(const QString &directoryToCodeToCompile, const QString &fileName)
{
	mPluginCompiler.compilePlugin(
			fileName
			, directoryToCodeToCompile
			, mQmakeParameter
			, mMakeParameter
			, mConfigurationParameter
	);
}

void MainClass::launchQrxc(const QString &fileName)
{
	QString tempPath = "plugins/editors/qrtest/qrxc";
	mQrxcLauncher.launchQrxc(fileName, mQRealRootPath, tempPath);
}

Metamodel* MainClass::loadedPlugin(const QString &fileName, const QString &pathToFile)
{
	return mPluginLoader.loadedPlugin(fileName, pathToFile, mPluginExtension, mPrefix);
}

void MainClass::createHtml(QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
		, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
		, QList<MethodsTester::ResultOfGenerating> timeResult
		, QList<MethodsTester::ResultOfGenerating> timeResultInterpter)
{
	mHtmlMaker.makeHtml(qrxcAndQrmcResult, qrxcAndInterpreterResult, timeResult, timeResultInterpter, mGeneratedDirHtml);
}

void MainClass::appendPluginNames()
{
	mQrxcGeneratedPluginsList.append(mPluginLoader.pluginNames());
}

void MainClass::parseConfigurationFile(const QString &fileName)
{
	mConfigurationFileParser.parseConfigurationFile(fileName);

	mQmakeParameter = mConfigurationFileParser.qmakeParameter();
	mMakeParameter = mConfigurationFileParser.makeParameter();
	mConfigurationParameter = mConfigurationFileParser.configurationParameter();
	mPluginExtension = mConfigurationFileParser.pluginExtension();
	mPrefix = mConfigurationFileParser.prefix();
	mQRealRootPath = mConfigurationFileParser.qRealRootPath();
	mGenerateHtml = mConfigurationFileParser.htmlGenerationParameter();
	mGeneratedCodeDirQrxc = mConfigurationFileParser.generatedCodeDirQrxc();
	mGeneratedCodeDirQrmc = mConfigurationFileParser.generatedCodeDirQrmc();
	mGeneratedDirHtml = mConfigurationFileParser.generatedDirHtml();
}

