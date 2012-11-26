#include "mainClass.h"
#include "methodsTester.h"
#include <QtCore/QDir>

MainClass::MainClass(QString &fileName, QString const &pathToQrmc, QString const &pathToApp)
{
	// launching and compiling qrmc
	createNewFolders();
	QString normalizedFileName = normalizedName(fileName);
	launchQrmc(fileName, pathToQrmc);
	compilePlugin("../qrtest/binaries/sources/qrmc"); // from launchqrmc
	QString const &pathToQrmcGenerated = "../qrtest/binaries/plugins/qrmc";
	EditorInterface* qrmcGeneratedPlugin = loadedPlugin(normalizedFileName, pathToApp, pathToQrmcGenerated);

	launchQrxc(normalizedFileName);
	QString const &pathToQrxcGenerated = "../qrtest/binaries/plugins/qrxc";
	EditorInterface* qrxcGeneratedPlugin = loadedPlugin(normalizedFileName, pathToApp, pathToQrxcGenerated);

	MethodsTester* methodsTester = new MethodsTester(qrmcGeneratedPlugin, qrxcGeneratedPlugin);

	methodsTester->testMethods();
}

void MainClass::createNewFolders()
{
	QDir dir;
	if (!dir.exists("../qrtest/binaries")) {
		dir.mkdir("../qrtest/binaries");
	}

	if (!dir.exists("../qrtest/binaries/sources")) {
		dir.mkdir("../qrtest/binaries/sources");
	}

	if (!dir.exists("../qrtest/binaries/plugins")) {
		dir.mkdir("../qrtest/binaries/plugins");
	}
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
	mQrmcLauncher.launchQrmc(fileName, "../qrtest/binaries/sources/qrmc", pathToQrmc);
}

void MainClass::compilePlugin(QString const &directoryToCodeToCompile)
{
	mPluginCompiler.compilePlugin(directoryToCodeToCompile);
}

void MainClass::launchQrxc(QString &fileName)
{
	mQrxcLauncher.launchQrxc(fileName);
}

EditorInterface* MainClass::loadedPlugin(QString const &fileName, QString const &pathToApp, QString const &pathToFile)
{
	return mPluginLoader.loadedPlugin(fileName, pathToApp, pathToFile);
}

