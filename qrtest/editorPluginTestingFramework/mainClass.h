#pragma once

#include "qrmcLauncher.h"
#include "pluginCompiler.h"
#include "pluginLoader.h"
#include "qrxcLauncher.h"
#include "htmlMaker.h"

#include "../../qrgui/editorPluginInterface/editorInterface.h"

#include <QtCore/QString>

namespace editorPluginTestingFramework {

class MainClass
{
public:
	MainClass(QString const &fileName, QString const &pathToQrmc);

private:
	static void createNewFolders();
	static QString normalizedName(QString const &fileName);
	static void createFolder(QString const &path);
	static void deleteOldBinaries(QString const &directory);

	void launchQrmc(QString const &fileName, QString const &pathToQrmc);
	void compilePlugin(QString const &directoryToCodeToCompile);
	void launchQrxc(QString const &fileName);
	void createHtml(QList<QPair<QString, QString> > outputList);

	qReal::EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);

	QrmcLauncher mQrmcLauncher;
	PluginCompiler mPluginCompiler;
	PluginLoader mPluginLoader;
	QrxcLauncher mQrxcLauncher;
	HtmlMaker mHtmlMaker;
};

}

