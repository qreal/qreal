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

void QrxcLauncher::launchQrxc(QString const &fileName)
{
	QString normalizedFileName = fileName;

	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	RepoApi const *mRepoApi = new RepoApi(normalizedFileName);
	ErrorReporter * const reporter = new ErrorReporter();
	EditorGenerator editorGenerator(*mRepoApi, *reporter);

	QDir dir(".");

	QHash<Id, QString> metamodelList = editorGenerator.getMetamodelList();

	dir.mkpath(pathToQrxcGeneratedCode);
	foreach (Id const &key, metamodelList.keys()) {
		if (mRepoApi->isLogicalElement(key)) {

			QString const pathToQRealRoot = mRepoApi->stringProperty(key, "path to QReal Source Files");

			QString const &directoryToGeneratedCode = generatePathToPlugin(pathToQRealRoot);

			if (!dir.exists(directoryToGeneratedCode)) {
				dir.mkdir(directoryToGeneratedCode);
			}

			QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key
					, pathToQrxcGeneratedCode
					, pathToQRealRoot
					, directoryToGeneratedCode);
		}
	}
}

QString QrxcLauncher::generatePathToPlugin(QString const &pathToQRealRoot)
{
	QString result;

	QString pathToEditorsSdk = pathToQRealRoot + "/plugins";
	int const levels = pathToEditorsSdk.split("/", QString::SkipEmptyParts).count();

	for (int i = 0; i < levels; i++) {
		result += "../";
	}

	result += pathToQrxcGeneratedPlugin;

	return result;
}
