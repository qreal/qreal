#include "constraintsGeneratorPlugin.h"
#include <QtGui/QMessageBox>
#include <QtGui/QProgressBar>
#include <QtCore/QProcess>
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include "../../../qrkernel/settingsManager.h"

Q_EXPORT_PLUGIN2(ConstraintsGeneratorPlugin, constraints::generator::ConstraintsGeneratorPlugin)

using namespace constraints::generator;
using namespace qReal;

ConstraintsGeneratorPlugin::ConstraintsGeneratorPlugin()
{
	mAppTranslator.load(":/constraintsGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

ConstraintsGeneratorPlugin::~ConstraintsGeneratorPlugin()
{
}

void ConstraintsGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModel = &configurator.logicalModelApi();
	mGenerator.init(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
}

QList<ActionInfo> ConstraintsGeneratorPlugin::actions()
{
	QAction * const generateAction = new QAction(QObject::tr("Generate constraints"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}

void ConstraintsGeneratorPlugin::generate()
{
	foreach (qReal::Id const &metamodel, mLogicalModel->logicalRepoApi().elementsByType("MetamodelConstraints")) {
		if (!mLogicalModel->logicalRepoApi().isLogicalElement(metamodel)) {
			continue;
		}
		mGenerator.generate(metamodel);

		QString const constraintModelFullName =  mGenerator.constraintModelFullName();
		QString const constraintModelName = mGenerator.constraintModelName();
		QString const constraintNormalizerModelName = mGenerator.constraintNormalizerModelName();
		QString const constraintModelId = mGenerator.constraintModelId();

		QPair<QString, QString> const constraintModelNames = QPair<QString, QString>(constraintModelName, constraintNormalizerModelName);

		if (!mMainWindowInterface->errorReporter()->wereErrors()) {
			if (QMessageBox::question(mMainWindowInterface->windowWidget()
									  , tr("loading.."), QString(tr("Do you want to load generated constraints?")),
									  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
			loadNewEditor(constraintModelFullName, constraintModelNames, constraintModelId
						  , SettingsManager::value("pathToQmake", "").toString()
						  , SettingsManager::value("pathToMake", "").toString()
						  , SettingsManager::value("pluginExtension", "").toString()
						  , SettingsManager::value("prefix", "").toString());
		}
	}
}

void ConstraintsGeneratorPlugin::loadNewEditor(QString const &directoryName
		, QPair<QString, QString> const &pluginsNames
		, QString const &pluginId
		, QString const &commandFirst
		, QString const &commandSecond
		, QString const &extension
		, QString const &prefix)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	QString const pluginName = pluginsNames.first;
	QString const normalizerPluginName = pluginsNames.second;

	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("please, fill compiler settings"));
		return;
	}

	QProgressBar * const progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();

	QApplication::processEvents();

	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	if (!mMainWindowInterface->unloadConstraintsPlugin(pluginName, pluginId)) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot unload plugin"));
		deleteGeneratedFiles(directoryName, normalizerPluginName);
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

			if (mMainWindowInterface->loadConstraintsPlugin(prefix + pluginName + "." + extension)) {
				progress->setValue(100);
			}
		}
	}

	if (progress->value() != 100) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot load new constraints plugin"));
		deleteGeneratedFiles(directoryName, normalizerPluginName);
	}
	progress->setValue(100);
	progress->close();
	delete progress;
}

void ConstraintsGeneratorPlugin::deleteGeneratedFiles(QString const &directoryName, QString const &fileBaseName)
{
	QFile filePro(directoryName + "/" + fileBaseName + ".pro");
	QFile fileXml(directoryName + "/" + fileBaseName + ".xml");
	filePro.remove();
	fileXml.remove();
}
