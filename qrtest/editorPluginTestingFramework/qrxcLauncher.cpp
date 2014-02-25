#include "qrxcLauncher.h"

#include "plugins/metaEditor/metaEditorSupport/editorGenerator.h"
#include "qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "qrrepo/repoApi.h"
#include "qrgui/mainwindow/errorReporter.h"
#include "qrxc/xmlCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrRepo;
using namespace metaEditor;
using namespace gui;
using namespace editorPluginTestingFramework;

void QrxcLauncher::launchQrxc(QString const &fileName, QString const &pathToQRealSources, QString const &pathToGeneratedCode)
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

	dir.mkpath(pathToGeneratedCode + pathToQrxcGeneratedCode);
	foreach (Id const &key, metamodelList.keys()) {
		if (mRepoApi->isLogicalElement(key)) {
			QString const &directoryToGeneratedCode = generatePathToPlugin(pathToGeneratedCode);

			if (!dir.exists(directoryToGeneratedCode)) {
				dir.mkdir(directoryToGeneratedCode);
			}

			QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key
					, pathToGeneratedCode + pathToQrxcGeneratedCode
					, pathToQRealSources
					, directoryToGeneratedCode);
		}
	}
	qDebug() << stringSeparator;
}

QString QrxcLauncher::generatePathToPlugin(QString const &pathToGeneratedCode)
{
	QString result;

	for (int i = 0; i < 3; i++) {
		result += "../";
	}

	result += pathToGeneratedCode + pathToQrxcGeneratedPlugin;

	return result;
}
