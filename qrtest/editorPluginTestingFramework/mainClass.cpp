#include "mainClass.h"
#include "methodsTester.h"

MainClass::MainClass(QString &fileName, QString const &pathToQrmc, QString const &pathToApp)
{
	// launching and compiling qrmc
	launchQrmc(fileName, pathToQrmc);
	compilePlugin("../qrtest/editorPluginTestingFramework/plugins"); // from launchqrmc
	QString const &pathToQrmcGenerated = "../qrtest/bin/plugins/";
	EditorInterface* qrmcGeneratedPlugin = loadedPlugin(fileName, pathToApp, pathToQrmcGenerated);

	launchQrxc(fileName);
	QString const &pathToQrxcGenerated = "../qrtest/bin/qrtest/bin/qrxc";
	EditorInterface* qrxcGeneratedPlugin = loadedPlugin(fileName, pathToApp, pathToQrxcGenerated);

	MethodsTester* methodsTester = new MethodsTester(qrmcGeneratedPlugin, qrxcGeneratedPlugin);
	methodsTester->testMethods();
}

void MainClass::launchQrmc(QString &fileName, QString const &pathToQrmc)
{
	mQrmcLauncher.launchQrmc(fileName, "../qrtest/editorPluginTestingFramework/plugins", pathToQrmc);
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

