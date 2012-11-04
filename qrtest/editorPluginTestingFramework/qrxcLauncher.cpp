#include "qrxcLauncher.h"

#include "../../plugins/metaEditor/metaEditorSupport/editorGenerator.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "../../qrrepo/repoApi.h"
#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrxc/xmlCompiler.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace qrRepo;
using namespace metaEditor;
using namespace gui;

QrxcLauncher::QrxcLauncher()
{
}

void QrxcLauncher::launchQrxc(QString fileName)
{
	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}
	RepoApi const *mRepoApi = new RepoApi(normalizedFileName);
	ErrorReporter *reporter = new ErrorReporter();
	EditorGenerator editorGenerator(*mRepoApi, *reporter);

	QDir dir(".");

	QHash<Id, QPair<QString, QString> > metamodelList = editorGenerator.getMetamodelList();
	bool isEmpty = metamodelList.isEmpty();
	qDebug() << "List is empty" << isEmpty;

	QString const pathToQRealRoot = "../../.."; // i want to sleep
	foreach (Id const &key, metamodelList.keys()) {
		//QString const nameOfTheDirectory = metamodelList[key].first;
		//QString const pathToQRealRoot = metamodelList[key].second;
		QString nameOfTheDirectory = "../qrtest/bin/qrxc"; // sleeeeeeeeeep

		dir.mkpath(nameOfTheDirectory);
		QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key, nameOfTheDirectory, pathToQRealRoot, nameOfTheDirectory);
	}


}
