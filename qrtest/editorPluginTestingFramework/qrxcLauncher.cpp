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

void QrxcLauncher::launchQrxc(QString &fileName)
{
	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}
	RepoApi const *mRepoApi = new RepoApi(normalizedFileName);
	ErrorReporter *reporter = new ErrorReporter();
	EditorGenerator editorGenerator(*mRepoApi, *reporter);

	QDir dir(".");

	QHash<Id, QString > metamodelList = editorGenerator.getMetamodelList();
	bool isEmpty = metamodelList.isEmpty();
	qDebug() << "List is empty" << isEmpty;

	QString nameOfTheDirectory = "../qrtest/bin/qrxc"; // sleeeeeeeeeep
	QString const pathToQRealRoot = "../../.."; // i want to sleep
	foreach (Id const &key, metamodelList.keys()) {
		dir.mkpath(nameOfTheDirectory);
		QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key
				, nameOfTheDirectory
				, pathToQRealRoot
				, nameOfTheDirectory
				, fileName);
	}

	QProcess builder;
	builder.setWorkingDirectory(nameOfTheDirectory);

	builder.start(SettingsManager::value("pathToQmake").toString());
	qDebug()  << "qmake";
	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {

		builder.start(SettingsManager::value("pathToMake").toString());

		bool finished = builder.waitForFinished(100000);
		qDebug()  << "make";
		if (finished && (builder.exitCode() == 0)) {
			qDebug()  << "make ok";
		}
	}
}
