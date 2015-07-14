#include "qrxcLauncher.h"

#include "plugins/metaEditor/metaEditorSupport/editorGenerator.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h"
#include "qrrepo/repoApi.h"
#include "qrgui/mainwindow/errorReporter.h"
#include "qrxc/xmlCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrRepo;
using namespace metaEditor;
using namespace gui;
using namespace editorPluginTestingFramework;

void QrxcLauncher::launchQrxc(const QString &fileName, const QString &pathToQRealSources, const QString &pathToGeneratedCode)
{
	qDebug() << "STARTING QRXC LAUNCHING";
	QString normalizedFileName = fileName;

	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	const RepoApi *mRepoApi = new RepoApi(normalizedFileName);
	ErrorReporter * const reporter = new ErrorReporter();
	EditorGenerator editorGenerator(*mRepoApi, *reporter);

	QDir dir(".");

	QHash<qReal::Id, QPair<QString, QString>> metamodelList = editorGenerator.getMetamodelList();

	dir.mkpath(pathToGeneratedCode + pathToQrxcGeneratedCode);
	for (const Id &key : metamodelList.keys()) {
		if (mRepoApi->isLogicalElement(key)) {
			const QString &directoryToGeneratedCode = generatePathToPlugin(pathToGeneratedCode);

			if (!dir.exists(directoryToGeneratedCode)) {
				dir.mkdir(directoryToGeneratedCode);
			}

			QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key
					, pathToGeneratedCode + pathToQrxcGeneratedCode
					, pathToQRealSources);
		}
	}
	qDebug() << stringSeparator;
}

QString QrxcLauncher::generatePathToPlugin(const QString &pathToGeneratedCode)
{
	QString result;

	for (int i = 0; i < 3; i++) {
		result += "../";
	}

	result += pathToGeneratedCode + pathToQrxcGeneratedPlugin;

	return result;
}
