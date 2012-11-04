#include "mainClass.h"
#include "methodsTester.h"

MainClass::MainClass(QString fileName, QString const pathToQrmc, QString const pathToApp)
{
	// launching and compiling qrmc
	launchQrmc(fileName, pathToQrmc);
	compilePlugin("../qrtest/editorPluginTestingFramework/plugins");
	EditorInterface* qrmcGeneratedPlugin = loadedPlugin(fileName, pathToApp);
	MethodsTester* methodsTester = new MethodsTester(qrmcGeneratedPlugin);
	methodsTester->testMethods();

	launchQrxc(fileName);
}

void MainClass::launchQrmc(QString fileName, QString const pathToQrmc)
{
	mQrmcLauncher.launchQrmc(fileName, "../qrtest/editorPluginTestingFramework/plugins", pathToQrmc);
}

void MainClass::compilePlugin(QString const directoryToCodeToCompile)
{
	mPluginCompiler.compilePlugin(directoryToCodeToCompile);
}

void MainClass::launchQrxc(QString const &fileName)
{
	mQrxcLauncher.launchQrxc(fileName);
}

EditorInterface* MainClass::loadedPlugin(QString const &fileName, QString const &pathToApp)
{
	return mPluginLoader.loadedPlugin(fileName, pathToApp);
}

