#pragma once

#include "qrmcLauncher.h"
#include "pluginCompiler.h"
#include "pluginLoader.h"
#include "qrxcLauncher.h"

#include "../../qrgui/editorPluginInterface/editorInterface.h"

#include <QtCore/QString>

namespace editorPluginTestingFramework {

class MainClass
{
public:
	MainClass(QString const &fileName, QString const &pathToQrmc, QString const &pathToApp);

private:
	void createNewFolders();
	QString const normalizedName(QString const &fileName) const;

	void launchQrmc(QString const &fileName, QString const &pathToQrmc);
	void compilePlugin(QString const &directoryToCodeToCompile);
	void launchQrxc(QString const &fileName);

	void createFolder(QString const &path);

	qReal::EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);

	QrmcLauncher mQrmcLauncher;
	PluginCompiler mPluginCompiler;
	PluginLoader mPluginLoader;
	QrxcLauncher mQrxcLauncher;
};

}

