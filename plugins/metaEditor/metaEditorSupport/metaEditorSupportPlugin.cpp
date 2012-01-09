#include "metaEditorSupportPlugin.h"

#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressBar>
#include <QtGui/QDesktopWidget>

#include "editorGenerator.h"
#include "../../../qrkernel/settingsManager.h"

Q_EXPORT_PLUGIN2(metaEditorSupportPlugin, metaEditor::MetaEditorSupportPlugin)

using namespace qReal;
using namespace metaEditor;

MetaEditorSupportPlugin::MetaEditorSupportPlugin()
		: mGenerateEditorForQrxcAction(NULL)
		, mGenerateEditorWithQrmcAction(NULL)
		, mParseEditorXmlAction(NULL)
{
	mAppTranslator.load(":/metaEditorSupport_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

MetaEditorSupportPlugin::~MetaEditorSupportPlugin()
{
}

void MetaEditorSupportPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
}

QList<ActionInfo> MetaEditorSupportPlugin::actions()
{
	mGenerateEditorForQrxcAction.setText(tr("Generate editor"));
	ActionInfo generateEditorForQrxcActionInfo(&mGenerateEditorForQrxcAction, "generators", "tools");
	connect(&mGenerateEditorForQrxcAction, SIGNAL(triggered()), this, SLOT(generateEditorForQrxc()));

	mGenerateEditorWithQrmcAction.setText(tr("Generate editor (qrmc)"));
	ActionInfo generateEditorWithQrmcActionInfo(&mGenerateEditorWithQrmcAction, "generators", "tools");
	connect(&mGenerateEditorWithQrmcAction, SIGNAL(triggered()), this, SLOT(generateEditorWithQrmc()));

	mParseEditorXmlAction.setText(tr("Parse editor xml"));
	ActionInfo parseEditorXmlActionInfo(&mParseEditorXmlAction, "generators", "tools");
	connect(&mParseEditorXmlAction, SIGNAL(triggered()), this, SLOT(parseEditorXml()));

	return QList<ActionInfo>() << generateEditorForQrxcActionInfo << generateEditorWithQrmcActionInfo
			<< parseEditorXmlActionInfo;
}

void MetaEditorSupportPlugin::generateEditorForQrxc()
{
	EditorGenerator editorGenerator(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());

	QDir dir(".");

	QHash<Id, QPair<QString, QString> > metamodelList = editorGenerator.getMetamodelList();
	foreach (Id const &key, metamodelList.keys()) {
		QString const metamodelFullName = metamodelList[key].first;
		QString const pathToQRealRoot = metamodelList[key].second;
		dir.mkpath(metamodelFullName);
		QFileInfo const metamodelFileInfo(metamodelFullName);
		QString const metamodelName = metamodelFileInfo.baseName();
		editorGenerator.generateEditor(key, metamodelFullName + "/" + metamodelName, pathToQRealRoot);

		if (!mMainWindowInterface->errorReporter()->wereErrors()) {
			if (QMessageBox::question(mMainWindowInterface->windowWidget()
					, tr("loading.."), QString(tr("Do you want to load generated editor %1?")).arg(metamodelName),
					QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
			loadNewEditor(metamodelFullName, metamodelName
					, SettingsManager::value("pathToQmake", "").toString()
					, SettingsManager::value("pathToMake", "").toString()
					, SettingsManager::value("pluginExtension", "").toString()
					, SettingsManager::value("prefix", "").toString());
		}
	}
	if (metamodelList.isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(tr("There is nothing to generate"));
	}
}

void MetaEditorSupportPlugin::generateEditorWithQrmc()
{
}

void MetaEditorSupportPlugin::parseEditorXml()
{
}

void MetaEditorSupportPlugin::loadNewEditor(QString const &directoryName
		, QString const &metamodelName
		, QString const &commandFirst
		, QString const &commandSecond
		, QString const &extension
		, QString const &prefix)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("please, fill compiler settings"));
		return;
	}

	QString const normalizeDirName = metamodelName.at(0).toUpper() + metamodelName.mid(1);

	QProgressBar * const progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();

	QApplication::processEvents();

	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	if (mMainWindowInterface->unloadPlugin(normalizeDirName)) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot unload plugin"));
		progress->close();
		delete progress;
		return;
	}

	progress->setValue(20);

	QProcess builder;
	builder.setWorkingDirectory(directoryName);
	builder.start(commandFirst);

	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
		progress->setValue(60);
		builder.start(commandSecond);
		if (builder.waitForFinished() && (builder.exitCode() == 0)) {
			progress->setValue(80);

			if (mMainWindowInterface->loadPlugin(prefix + metamodelName + "." + extension, normalizeDirName)) {
				progress->setValue(100);
			}
		}
	}

	if (progress->value() != 100) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot load new editor"));
	}
	progress->setValue(100);
	progress->close();
	delete progress;
}
