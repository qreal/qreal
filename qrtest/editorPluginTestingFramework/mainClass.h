#include "qrmcLauncher.h"
#include "pluginCompiler.h"
#include "pluginLoader.h"
#include "qrxcLauncher.h"

#include "../../qrgui/editorPluginInterface/editorInterface.h"

#include <QtCore/QString>

class MainClass
{
public:
	MainClass(QString &fileName, QString const &pathToQrmc, QString const &pathToApp);

private:
	void createNewFolders();
	QString const normalizedName(QString const &fileName);

	void launchQrmc(QString &fileName, QString const &pathToQrmc);
	void compilePlugin(QString const &directoryToCodeToCompile);
	void launchQrxc(QString &fileName);

	void createFolder(QString const &path);

	EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);

	QrmcLauncher mQrmcLauncher;
	PluginCompiler mPluginCompiler;
	PluginLoader mPluginLoader;
	QrxcLauncher mQrxcLauncher;
};

