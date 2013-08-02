#pragma once

#include "qrmcLauncher.h"
#include "pluginCompiler.h"
#include "pluginLoader.h"
#include "qrxcLauncher.h"
#include "htmlMaker.h"
#include "configurationFileParser.h"

#include "../../qrrepo/repoApi.h"
#include "../../qrgui/pluginManager/interpreterEditorManager.h"

#include "../../qrgui/editorPluginInterface/editorInterface.h"

#include <QtCore/QString>

namespace editorPluginTestingFramework {

class MainClass
{
public:
	/// gets name of qrs-file and path to qrmc, launches all processes for testing
	MainClass(QString const &fileName, QString const &pathToQrmc);

private:
	/// creates qrtest/binaries and all inner folders
	static void createNewFolders();
	/// returns normalized name (without extension)
	static QString normalizedName(QString const &fileName);
	/// creates one folder
	static void createFolder(QString const &path);
	/// deletes old qrtest/binaries and all inner folders and files
	static void deleteOldBinaries(QString const &directory);

	void launchQrmc(QString const &fileName, QString const &pathToQrmc);
	void compilePlugin(QString const &directoryToCodeToCompile);
	void launchQrxc(QString const &fileName);
	void createHtml(QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult
			, QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult);

	/// puts names of generated plugins into mQrxcGeneratedPluginList (for further creation of EditorManager)
	void appendPluginNames();

	/// parses file with configuration settings
	void parseConfigurationFile();

	/// returns loaded editorInterface
	qReal::EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);

	QrmcLauncher mQrmcLauncher;
	PluginCompiler mPluginCompiler;
	PluginLoader mPluginLoader;
	QrxcLauncher mQrxcLauncher;

	QStringList mQrxcGeneratedPluginsList;

	HtmlMaker mHtmlMaker;
	ConfigurationFileParser mConfigurationFileParser;

	/// path to qmake
	QString mQmakeParameter;
	/// path to make
	QString mMakeParameter;
	/// configuration: debug or release
	QString mConfigurationParameter;
};

}

