#include "metaEditorSupportPlugin.h"

#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressBar>
#include <QtGui/QDesktopWidget>

#include "../../../qrkernel/settingsManager.h"
#include "../../../qrmc/metaCompiler.h"

#include "editorGenerator.h"
#include "xmlParser.h"

Q_EXPORT_PLUGIN2(metaEditorSupportPlugin, metaEditor::MetaEditorSupportPlugin)

using namespace qReal;
using namespace metaEditor;

MetaEditorSupportPlugin::MetaEditorSupportPlugin()
		: mGenerateEditorForQrxcAction(NULL)
		, mGenerateEditorWithQrmcAction(NULL)
		, mParseEditorXmlAction(NULL)
		, mRepoControlApi(NULL)
		, mCompilerSettingsPage(new PreferencesCompilerPage())
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
	mRepoControlApi = &configurator.repoControlInterface();
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

QPair<QString, PreferencesPage *> MetaEditorSupportPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Compiler"), static_cast<PreferencesPage*>(mCompilerSettingsPage));
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
	qrmc::MetaCompiler metaCompiler(qApp->applicationDirPath() + "/../qrmc", mRepoControlApi->workingFile());

	IdList const metamodels = mLogicalRepoApi->children(Id::rootId());

	QProgressBar *progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	QApplication::processEvents();
	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);

	int forEditor = 60 / metamodels.size();

	foreach (Id const &key, metamodels) {
		QString const objectType = key.element();
		if (objectType == "MetamodelDiagram") {
			QString name = mLogicalRepoApi->stringProperty(key, "name of the directory");
			if (QMessageBox::question(mMainWindowInterface->windowWidget()
					, tr("loading..")
					, QString(tr("Do you want to compile and load editor %1?")).arg(name)
					, QMessageBox::Yes, QMessageBox::No)
					== QMessageBox::No)
			{
				continue;
			}

			progress->setValue(5);

			if (!metaCompiler.compile(name)) { // generating source code for all metamodels
				QMessageBox::warning(mMainWindowInterface->windowWidget()
						, tr("error")
						, tr("Cannot generate source code for editor ") + name);
				continue;
			}
			progress->setValue(20);

			QProcess builder;
			builder.setWorkingDirectory("../qrmc/plugins");
			builder.start(SettingsManager::value("pathToQmake", "").toString());
			qDebug()  << "qmake";
			if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
				progress->setValue(40);

				builder.start(SettingsManager::value("pathToMake", "").toString());

				bool finished = builder.waitForFinished(100000);
				qDebug()  << "make";
				if (finished && (builder.exitCode() == 0)) {
					qDebug()  << "make ok";

					progress->setValue(progress->value() + forEditor / 2);

					QString normalizedName = name.at(0).toUpper() + name.mid(1);
					if (!name.isEmpty()) {
						if (!mMainWindowInterface->unloadPlugin(normalizedName)) {
							QMessageBox::warning(mMainWindowInterface->windowWidget()
									, tr("error")
									, tr("cannot unload plugin ") + normalizedName);
							progress->close();
							delete progress;
							continue;
						}
					}

					QString const generatedPluginFileName = SettingsManager::value("prefix", "").toString()
							+ name
							+ "."
							+ SettingsManager::value("pluginExtension", "").toString()
							;
					if (mMainWindowInterface->loadPlugin(generatedPluginFileName, normalizedName)) {
						progress->setValue(progress->value() + forEditor / 2);
					}
				}
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

void MetaEditorSupportPlugin::parseEditorXml()
{
	if (!mMainWindowInterface->pluginLoaded("MetaEditor")) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("required plugin (MetaEditor) is not loaded"));
		return;
	}
	QDir dir(".");
	QString directoryName = ".";
	while (dir.cdUp()) {
		QFileInfoList const infoList = dir.entryInfoList(QDir::Dirs);
		foreach (QFileInfo const &directory, infoList){
			if (directory.baseName() == "qrxml") {
				directoryName = directory.absolutePath() + "/qrxml";
			}
		}
	}
	QString const fileName = QFileDialog::getOpenFileName(mMainWindowInterface->windowWidget()
			, tr("Select xml file to parse")
			, directoryName
			, "XML files (*.xml)");

	if (fileName.isEmpty())
		return;

	XmlParser parser(*mLogicalRepoApi);

	parser.parseFile(fileName);

	parser.loadIncludeList(fileName);

	mMainWindowInterface->reinitModels();
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
