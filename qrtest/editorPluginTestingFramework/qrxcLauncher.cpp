#include "qrxcLauncher.h"

#include "../../plugins/metaEditor/metaEditorSupport/editorGenerator.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "../../qrrepo/repoApi.h"
#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrxc/xmlCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrRepo;
using namespace metaEditor;
using namespace gui;
using namespace editorPluginTestingFramework;

void QrxcLauncher::launchQrxc(QString const &fileName, QString const &pathToQRealSources)
{
	qDebug() << "STARTING QRXC LAUNCHING";
	QString normalizedFileName = fileName;

	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	RepoApi const *mRepoApi = new RepoApi(normalizedFileName);
	ErrorReporter * const reporter = new ErrorReporter();
	EditorGenerator editorGenerator(*mRepoApi, *reporter);

	QDir dir(".");

	QHash<Id, QString> metamodelList = editorGenerator.getMetamodelList(pathToQRealSources);

	dir.mkpath(pathToQrxcGeneratedCode);
	foreach (Id const &key, metamodelList.keys()) {
		if (mRepoApi->isLogicalElement(key)) {
			/*SettingsManager::value("pathToQRealSourceFiles").toString();*/
			QString const &directoryToGeneratedCode = generatePathToPlugin(pathToQRealSources);

			if (!dir.exists(directoryToGeneratedCode)) {
				dir.mkdir(directoryToGeneratedCode);
			}

			QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key
					, pathToQrxcGeneratedCode
					, pathToQRealSources
					, directoryToGeneratedCode);
		}
	}
	qDebug() << stringSeparator;
}

QString QrxcLauncher::generatePathToPlugin(QString const &pathToQRealRoot)
{
	QString result;

	QString pathToEditorsSdk = pathToQRealRoot + "/plugins";
	int const levels = pathToEditorsSdk.split("/", QString::SkipEmptyParts).count();

	for (int i = 0; i < levels - 1; i++) {
		result += "../";
	}

	result += pathToQrxcGeneratedPlugin;

	return result;
}
