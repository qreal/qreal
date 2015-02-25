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
		QString const &fileName
		, QString const &pathToQrmc
		, QString const &applicationPath
		, QString const &configurationFileName)
		: mTempOldValue(SettingsManager::value("temp").toString())
		, mApplicationPath(applicationPath)
{
	setTempValueInSettingsManager();

	qDebug() << "configuration file: " << configurationFileName;
	parseConfigurationFile(configurationFileName);

	deleteOldBinaries(mGeneratedCodeDir);
	createNewFolders();
	QString const normalizedFileName = normalizedName(fileName);

	copyTestMetamodel(fileName);

	launchQrxc(normalizedFileName);
	compilePlugin(mGeneratedCodeDir + pathToQrxcGeneratedCode);
	EditorInterface* const qrxcGeneratedPlugin = loadedPlugin(normalizedFileName, mGeneratedCodeDir + pathToQrxcGeneratedPlugin);
	appendPluginNames();

	launchQrmc(fileName, pathToQrmc);
	compilePlugin(mGeneratedCodeDir + pathToQrmcGeneratedCode);
	EditorInterface* const qrmcGeneratedPlugin = loadedPlugin(normalizedFileName, mGeneratedCodeDir + pathToQrmcGeneratedPlugin);

	InterpreterEditorManager interpreterEditorManager(fileName);
	EditorManager qrxcEditorManager(nullptr);
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
		if (mGenerateHtml == "yes") {
			createHtml(methodsTesterOutput, interpreterMethodsTesterOutput);
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

void MainClass::createFolder(QString const &path)
{
	QDir dir;
	if (!dir.exists(path)) {
		dir.mkdir(path);
	}
}

void MainClass::createNewFolders()
{
	createFolder(mGeneratedCodeDir);
	createFolder(mGeneratedCodeDir + pluginsDir);
	createFolder(mGeneratedCodeDir + sourcesDir);
	createFolder(mGeneratedCodeDir + pathToQrmcGeneratedPlugin);
	createFolder(mGeneratedCodeDir + pathToQrxcGeneratedPlugin);
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
	mQrmcLauncher.launchQrmc(fileName, pathToQrmc, mGeneratedCodeDir);
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
	mQrxcLauncher.launchQrxc(fileName, mApplicationPath, mGeneratedCodeDir);
}

EditorInterface* MainClass::loadedPlugin(QString const &fileName, QString const &pathToFile)
{
	return mPluginLoader.loadedPlugin(fileName, pathToFile, mPluginExtension, mPrefix);
}

void MainClass::createHtml(QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult
		, QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult)
{
	mHtmlMaker.makeHtml(qrxcAndQrmcResult, qrxcAndInterpreterResult, mGeneratedCodeDir);
}

void MainClass::appendPluginNames()
{
	mQrxcGeneratedPluginsList.append(mPluginLoader.pluginNames());
}

void MainClass::parseConfigurationFile(QString const &fileName)
{
	mConfigurationFileParser.parseConfigurationFile(fileName);

	mQmakeParameter = mConfigurationFileParser.qmakeParameter();
	mMakeParameter = mConfigurationFileParser.makeParameter();
	mConfigurationParameter = mConfigurationFileParser.configurationParameter();
	mPluginExtension = mConfigurationFileParser.pluginExtension();
	mPrefix = mConfigurationFileParser.prefix();
	mQRealRootPath = mConfigurationFileParser.qRealRootPath();
	mGenerateHtml = mConfigurationFileParser.htmlGenerationParameter();
	mGeneratedCodeDir = mConfigurationFileParser.generatedCodeDir();
}

