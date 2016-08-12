/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include <qrutils/xmlUtils.h>
#include <qrkernel/exception/exception.h>

#include "defs.h"
#include "methodsTesterForQrxcAndQrmc.h"
#include "methodsTesterForQrxcAndInterpreter.h"
#include "methodsCheckerForTravis.h"

#include "qrgui/plugins/pluginManager/interpreterEditorManager.h"
#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/plugins/pluginManager/editorManager.h"

using namespace qReal;
using namespace editorPluginTestingFramework;
using namespace qrRepo;

MainClass::MainClass(const QString &metamodelFileName
		, const QString &pathToQrmc
		, const QString &applicationPath
		, const QString &configurationFileName)
		: mTempOldValue(SettingsManager::value("temp").toString())
		, mApplicationPath(applicationPath)
{
	setTempFolderValue();

	qDebug() << "configuration file: " << configurationFileName;
	mConfiguration.parseConfigurationFile(configurationFileName);

	deleteOldFiles(mConfiguration.generatedCodeDirQrxc());
	deleteOldFiles(mConfiguration.generatedCodeDirQrmc());

	const QString normalizedFileName = normalizedName(metamodelFileName);

	launchEditorGenerator(metamodelFileName, mConfiguration.generatedCodeDirQrxc());
	const QFileInfo qrxcPluginBinaryInfo = compilePlugin(normalizedFileName, PluginCompiler::MetamodelCompiler::qrxc);
	Metamodel * const qrxcGeneratedPlugin = loadPlugin(qrxcPluginBinaryInfo);

//	appendPluginNames();

	launchQrmc(metamodelFileName);
	const QFileInfo qrmcPluginBinaryInfo = compilePlugin(normalizedFileName, PluginCompiler::MetamodelCompiler::qrmc);
	Metamodel * const qrmcGeneratedPlugin = loadPlugin(qrmcPluginBinaryInfo);

	/*
	InterpreterEditorManager interpreterEditorManager(metamodelFileName, nullptr);
	const QString path = "plugins/editors/qrtest/qrxc/plugins";
	EditorManager qrxcEditorManager(path);
	// we cast qrxc plugin to Editor Manager

	MethodsTesterForQrxcAndInterpreter* const interpreterMethodsTester = new MethodsTesterForQrxcAndInterpreter(
			&qrxcEditorManager
			,&interpreterEditorManager);

	QList<MethodsTester::ResultOfGenerating> interpreterMethodsTesterOutput =
			interpreterMethodsTester->generatedResult();

	QList<MethodsTester::ResultOfGenerating> methodsTimeOutputInterpreter =
			interpreterMethodsTester->generateTimeResult();

	if ((qrxcGeneratedPlugin != nullptr) && (qrmcGeneratedPlugin != nullptr)) {
		MethodsTesterForQrxcAndQrmc * const methodsTester = new MethodsTesterForQrxcAndQrmc(
				qrmcGeneratedPlugin, qrxcGeneratedPlugin);

		const QList<MethodsTester::ResultOfGenerating> methodsTesterOutput = methodsTester->generatedOutput();
		const QList<MethodsTester::ResultOfGenerating> methodsTimeOutput = methodsTester->generateTimeResult();

		if (mGenerateHtml == "yes") {
			createHtml(methodsTesterOutput, interpreterMethodsTesterOutput, methodsTimeOutput
					, methodsTimeOutputInterpreter);
		}

		mResultOfTesting = MethodsCheckerForTravis::calculateResult(methodsTesterOutput
				, interpreterMethodsTesterOutput);
	} else {
		qDebug() << "Generation of plugins failed";
	}
*/

	restoreOldValueOfTempFolder();
}

int MainClass::testResult() const
{
	return mResultOfTesting;
}

QString MainClass::normalizedName(const QString &fileName)
{
	return QFileInfo(fileName).baseName();
}

void MainClass::deleteOldFiles(const QString &directory)
{
	if (directory.isEmpty()) {
		throw qReal::Exception("Trying to delete everything in application directory, possibly error in a config file");
	}

	qDebug() << "Wiping directory" << directory;

	QDir dir(directory);
	if (!dir.exists()) {
		return;
	}

	for (const QFileInfo &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			deleteOldFiles(fileInfo.filePath());
			dir.rmdir(fileInfo.fileName());
		} else {
			dir.remove(fileInfo.fileName());
		}
	}
}

void MainClass::setTempFolderValue()
{
	SettingsManager::setValue("temp", mApplicationPath + tempValueForSettingsManager);
}

void MainClass::restoreOldValueOfTempFolder() const
{
	SettingsManager::setValue("temp", mTempOldValue);
}

void MainClass::launchQrmc(const QString &fileName)
{
	QrmcLauncher::launchQrmc(fileName, mConfiguration.generatedCodeDirQrmc());
}

QFileInfo MainClass::compilePlugin(const QString &fileWithMetamodel
		, PluginCompiler::MetamodelCompiler metamodelCompiler)
{
	return PluginCompiler::compilePlugin(
			fileWithMetamodel
			, metamodelCompiler == PluginCompiler::MetamodelCompiler::qrxc
					? mConfiguration.generatedCodeDirQrxc()
					: mConfiguration.generatedCodeDirQrmc()
			, mConfiguration.qmakeParameter()
			, mConfiguration.makeParameter()
			, mConfiguration.configurationParameter()
			, metamodelCompiler
	);
}

void MainClass::launchEditorGenerator(const QString &fileName, const QString &generatedXmlForQrxcDir)
{
	MetamodelXmlGeneratorLauncher::launchEditorGenerator(fileName, mConfiguration.qRealRootPath()
			, generatedXmlForQrxcDir);
}

Metamodel* MainClass::loadPlugin(const QFileInfo &file)
{
	return mPluginLoader.loadPlugin(file);
}

void MainClass::createHtml(QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
		, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
		, QList<MethodsTester::ResultOfGenerating> timeResult
		, QList<MethodsTester::ResultOfGenerating> timeResultInterpter)
{
//	mHtmlMaker.makeHtml(qrxcAndQrmcResult, qrxcAndInterpreterResult, timeResult
//			, timeResultInterpter, mGeneratedDirHtml);
}

//void MainClass::appendPluginNames()
//{
//	mQrxcGeneratedPluginsList.append(mPluginLoader.pluginNames());
//}
