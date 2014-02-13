#include "mainClass.h"
#include "defs.h"
#include "methodsTesterForQrxcAndQrmc.h"
#include "methodsTesterForQrxcAndInterpreter.h"
#include "methodsCheckerForTravis.h"

#include "qrgui/pluginManager/interpreterEditorManager.h"
#include "qrgui/pluginManager/editorManagerInterface.h"
#include "qrgui/pluginManager/editorManager.h"
#include "qrutils/xmlUtils.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

using namespace qReal;
using namespace editorPluginTestingFramework;
using namespace qrRepo;

MainClass::MainClass(
		QString const &fileName
		, QString const &pathToQrmc
		, QString const &applicationPath
		, bool const &travisMode)
		: mTempOldValue(SettingsManager::value("temp").toString())
		, mApplicationPath(applicationPath)
{
	setTempValueInSettingsManager();

	deleteOldBinaries(binariesDir);
	createNewFolders();
	QString const normalizedFileName = normalizedName(fileName);

	if (travisMode) {
		copyTestMetamodel(fileName);
	}

	parseConfigurationFile(travisMode);

	launchQrxc(normalizedFileName);
	compilePlugin(pathToQrxcGeneratedCode);
	EditorInterface* const qrxcGeneratedPlugin = loadedPlugin(normalizedFileName, pathToQrxcGeneratedPlugin);
	appendPluginNames();

	launchQrmc(fileName, pathToQrmc);
	compilePlugin(pathToQrmcGeneratedCode);
	EditorInterface* const qrmcGeneratedPlugin = loadedPlugin(normalizedFileName, pathToQrmcGeneratedPlugin);

	InterpreterEditorManager interpreterEditorManager(fileName);
	EditorManager qrxcEditorManager(destDirForQrxc, mQrxcGeneratedPluginsList);
	// we cast qrxc plugin to Editor Manager

	MethodsTesterForQrxcAndInterpreter* const interpreterMethodsTester = new MethodsTesterForQrxcAndInterpreter(
			&qrxcEditorManager
			,&interpreterEditorManager);

	QList<QPair<QString, QPair<QString, QString> > > interpreterMethodsTesterOutput =
			interpreterMethodsTester->generatedResult();

	if ((qrxcGeneratedPlugin != NULL) && (qrmcGeneratedPlugin != NULL)) {
		MethodsTesterForQrxcAndQrmc* const methodsTester = new MethodsTesterForQrxcAndQrmc(
				qrmcGeneratedPlugin, qrxcGeneratedPlugin);

		QList<QPair<QString, QPair<QString, QString> > > methodsTesterOutput = methodsTester->generatedOutput();
		if (!travisMode) {
			createHtml(methodsTesterOutput, interpreterMethodsTesterOutput);
		} else {
			mResultOfTesting = MethodsCheckerForTravis::calculateResult(methodsTesterOutput
					, interpreterMethodsTesterOutput);
		}
	} else {
		qDebug() << "Generation of plugins failed";
	}

	returnOldValueOfTemp();
}

int MainClass::travisTestResult() const
{
	return mResultOfTesting;
}

void MainClass::createFolder(QString const &path)
{
	QDir dir;
	if (!dir.exists(path)) {
		dir.mkdir(path);
	}
}

void MainClass::createNewFolders()
{
	createFolder(binariesDir);
	createFolder(pluginsDir);
	createFolder(sourcesDir);
	createFolder(pathToQrmcGeneratedPlugin);
	createFolder(pathToQrxcGeneratedPlugin);
}

QString MainClass::normalizedName(QString const &fileName)
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

void MainClass::deleteOldBinaries(QString const &directory)
{
	QDir dir(directory);
	if (!dir.exists()) {
		return;
	}

	foreach (QFileInfo const &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			deleteOldBinaries(fileInfo.filePath());
			dir.rmdir(fileInfo.fileName());
		} else {
			dir.remove(fileInfo.fileName());
		}
	}
}

void MainClass::copyTestMetamodel(QString const &fileName)
{
	QString const workingDirName = pathToTestMetamodel;
	QDir sourceDir(workingDirName);

	QFileInfo const destDirInfo;
	QDir destDir = destDirInfo.dir();

	QFile::copy(sourceDir.absolutePath() + "/" + fileName, destDir.absolutePath() + "/" + fileName);
}

void MainClass::setTempValueInSettingsManager()
{
	mApplicationPath.chop(4);
	SettingsManager::setValue("temp", mApplicationPath + tempValueForSettingsManager);
}

void MainClass::returnOldValueOfTemp() const
{
	SettingsManager::setValue("temp", mTempOldValue);
}

void MainClass::launchQrmc(QString const &fileName, QString const &pathToQrmc)
{
	mQrmcLauncher.launchQrmc(fileName, pathToQrmc);
}

void MainClass::compilePlugin(QString const &directoryToCodeToCompile)
{
	mPluginCompiler.compilePlugin(
			directoryToCodeToCompile
			, mQmakeParameter
			, mMakeParameter
			, mConfigurationParameter
	);
}

void MainClass::launchQrxc(QString const &fileName)
{
	mQrxcLauncher.launchQrxc(fileName, mApplicationPath);
}

EditorInterface* MainClass::loadedPlugin(QString const &fileName, QString const &pathToFile)
{
	return mPluginLoader.loadedPlugin(fileName, pathToFile, mPluginExtension, mPrefix);
}

void MainClass::createHtml(QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult
		, QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult)
{
	mHtmlMaker.makeHtml(qrxcAndQrmcResult, qrxcAndInterpreterResult);
}

void MainClass::appendPluginNames()
{
	mQrxcGeneratedPluginsList.append(mPluginLoader.pluginNames());
}

void MainClass::parseConfigurationFile(const bool &travisMode)
{
	if (travisMode) {
		mConfigurationFileParser.parseConfigurationFile(travisConfigurationFileName);
	} else {
		mConfigurationFileParser.parseConfigurationFile(configurationFileName);
	}
	mQmakeParameter = mConfigurationFileParser.qmakeParameter();
	mMakeParameter = mConfigurationFileParser.makeParameter();
	mConfigurationParameter = mConfigurationFileParser.configurationParameter();
	mPluginExtension = mConfigurationFileParser.pluginExtension();
	mPrefix = mConfigurationFileParser.prefix();
}

