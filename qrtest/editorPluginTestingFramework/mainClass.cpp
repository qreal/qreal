#include "mainClass.h"
#include "methodsTester.h"
#include "defs.h"
#include <QtCore/QDir>

MainClass::MainClass(QString &fileName, QString const &pathToQrmc, QString const &pathToApp)
{
	createNewFolders();
	QString normalizedFileName = normalizedName(fileName);

	launchQrmc(fileName, pathToQrmc);
	compilePlugin(pathToQrmcGeneratedCode);
	EditorInterface* qrmcGeneratedPlugin = loadedPlugin(normalizedFileName, pathToQrmcGeneratedPlugin);

	launchQrxc(normalizedFileName);
	compilePlugin(pathToQrxcGeneratedCode);
	EditorInterface* qrxcGeneratedPlugin = loadedPlugin(normalizedFileName, pathToQrxcGeneratedPlugin);

	MethodsTester* methodsTester = new MethodsTester(qrmcGeneratedPlugin, qrxcGeneratedPlugin);

	methodsTester->testMethods();
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

QString const MainClass::normalizedName(QString const &fileName)
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

void MainClass::launchQrmc(QString &fileName, QString const &pathToQrmc)
{
	mQrmcLauncher.launchQrmc(fileName, pathToQrmc);
}

void MainClass::compilePlugin(QString const &directoryToCodeToCompile)
{
	mPluginCompiler.compilePlugin(directoryToCodeToCompile);
}

void MainClass::launchQrxc(QString &fileName)
{
	mQrxcLauncher.launchQrxc(fileName);
}

EditorInterface* MainClass::loadedPlugin(QString const &fileName, QString const &pathToFile)
{
	return mPluginLoader.loadedPlugin(fileName, pathToFile);
}

