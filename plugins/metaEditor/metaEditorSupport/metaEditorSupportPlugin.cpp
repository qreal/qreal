#include "metaEditorSupportPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>

#include "../../../qrkernel/settingsManager.h"
#include "../../../qrmc/metaCompiler.h"

#include "editorGenerator.h"
#include "xmlParser.h"

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

	/*
	mParseEditorXmlAction.setText(tr("Parse editor xml")); // button for parsing xml, doesn't work
	ActionInfo parseEditorXmlActionInfo(&mParseEditorXmlAction, "generators", "tools");
	connect(&mParseEditorXmlAction, SIGNAL(triggered()), this, SLOT(parseEditorXml()));
	*/

	return QList<ActionInfo>() << generateEditorForQrxcActionInfo
	<< generateEditorWithQrmcActionInfo;
	//<< parseEditorXmlActionInfo;
}

QPair<QString, PreferencesPage *> MetaEditorSupportPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Compiler"), static_cast<PreferencesPage*>(mCompilerSettingsPage));
}

void MetaEditorSupportPlugin::generateEditorForQrxc()
{
	EditorGenerator editorGenerator(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());

	QDir dir(".");

	QString const pathToQrealRoot = SettingsManager::value("pathToQRealSourceFiles").toString();
	if (pathToQrealRoot.isEmpty()) {
		QMessageBox::information(NULL, tr("Path to source files is emtpy")
				, "Path to Qreal source files is empty. Please fill it in compiler settings.", tr("Ok"));
		return;
	}

	QHash<Id, QString > metamodelList = editorGenerator.getMetamodelList();
	foreach (Id const &key, metamodelList.keys()) {
		QString const nameOfTheDirectory = metamodelList[key];
		dir.mkpath(nameOfTheDirectory);
		QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key, nameOfTheDirectory, pathToQrealRoot);

		if (!mMainWindowInterface->errorReporter()->wereErrors()) {
			if (QMessageBox::question(mMainWindowInterface->windowWidget()
					, tr("loading.."), QString(tr("Do you want to load generated editor %1?")).arg(metamodelNames.first),
					QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
			loadNewEditor(nameOfTheDirectory, metamodelNames
					, SettingsManager::value("pathToQmake").toString()
					, SettingsManager::value("pathToMake").toString()
					, SettingsManager::value("pluginExtension").toString()
					, SettingsManager::value("prefix").toString()
					, SettingsManager::value("qmakeArguments").toString());
		}
	}
	if (metamodelList.isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(tr("There is nothing to generate"));
	}
}

void MetaEditorSupportPlugin::generateEditorWithQrmc()
{
	qrmc::MetaCompiler metaCompiler(qApp->applicationDirPath() + "/../qrmc", mLogicalRepoApi);

	IdList const metamodels = mLogicalRepoApi->children(Id::rootId());

	QString const pathToQrealRoot = SettingsManager::value("pathToQRealSourceFiles").toString();
	if (pathToQrealRoot.isEmpty()) {
		QMessageBox::information(NULL, tr("Path to source files is emtpy")
				, "Path to Qreal source files is empty. Please fill it in compiler settings.", tr("Ok"));
		return;
	}

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

	foreach (Id const &key, metamodels) {
		QString const objectType = key.element();
		if (objectType == "MetamodelDiagram" && mLogicalRepoApi->isLogicalElement(key)) {
			QString nameOfTheDirectory = mLogicalRepoApi->stringProperty(key, "name of the directory");
			QString nameOfMetamodel = mLogicalRepoApi->stringProperty(key, "name");
			QString nameOfPlugin = nameOfMetamodel.split("/").last();

			if (QMessageBox::question(mMainWindowInterface->windowWidget()
					, tr("loading..")
					, QString(tr("Do you want to compile and load editor %1?")).arg(nameOfPlugin)
					, QMessageBox::Yes, QMessageBox::No)
					== QMessageBox::No)
			{
				continue;
			}

			if (!metaCompiler.compile(nameOfMetamodel, pathToQrealRoot)) { // generating source code for all metamodels
				QMessageBox::warning(mMainWindowInterface->windowWidget()
						, tr("error")
						, tr("Cannot generate source code for editor ") + nameOfPlugin);
				continue;
			}

			QPair<QString, QString> metamodelNames = qMakePair(nameOfMetamodel, nameOfPlugin);
			loadNewEditor(nameOfTheDirectory, metamodelNames
					, SettingsManager::value("pathToQmake").toString()
					, SettingsManager::value("pathToMake").toString()
					, SettingsManager::value("pluginExtension").toString()
					, SettingsManager::value("prefix").toString()
					, SettingsManager::value("qmakeArguments").toString());
		}
	}

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
		, QPair<QString, QString> const &metamodelNames
		, QString const &commandFirst
		, QString const &commandSecond
		, QString const &extension
		, QString const &prefix
		, QString const &qmakeArguments)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	QString const metamodelName = metamodelNames.first;
	QString const normalizerMetamodelName = metamodelNames.second;

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

	if (!mMainWindowInterface->unloadPlugin(normalizeDirName)) {
		progress->close();
		delete progress;
		return;
	}

	progress->setValue(20);

	QProcess builder;
	builder.setWorkingDirectory(directoryName);
	if (!qmakeArguments.isEmpty()) {
		QStringList qmakeArgumentsList;
		qmakeArgumentsList.append("CONFIG+=" + qmakeArguments);
		builder.start(commandFirst, qmakeArgumentsList);
	} else {
		builder.start(commandFirst);
	}

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

	if (progress->value() == 20) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot qmake new editor"));
	} else if (progress->value() == 60) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot make new editor"));
	}

	progress->setValue(100);
	progress->close();
	delete progress;
}
