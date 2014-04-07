#include "generatorsGeneratorPlugin.h"
#include <QtCore/QProcess>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include "../../../qrkernel/settingsManager.h"

using namespace generators::generator;
using namespace qReal;

GeneratorsGeneratorPlugin::GeneratorsGeneratorPlugin()
{
	mAppTranslator.load(":/generatorsGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

GeneratorsGeneratorPlugin::~GeneratorsGeneratorPlugin()
{
}

void GeneratorsGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModel = &configurator.logicalModelApi();
	mGenerator.init(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());

}

QList<ActionInfo> GeneratorsGeneratorPlugin::actions()
{
	QAction * const generateAction = new QAction(tr("Generate generator"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}


void GeneratorsGeneratorPlugin::generate()
{
	foreach (qReal::Id const &metamodel, mLogicalModel->logicalRepoApi().elementsByType("GeneratorRoot")) {
		if (!mLogicalModel->logicalRepoApi().isLogicalElement(metamodel)) {
			continue;
		}
		mGenerator.generate(metamodel);

		QString const generatorModelFullName =  mGenerator.generatorModelFullName();
		QString const generatorModelName = mGenerator.generatorGeneratorModelName();
		QString const generatorNormalizerModelName = mGenerator.generatorNormalizerGeneratorModelName();
		QString const generatorModelId = mGenerator.generatorModelId();

		QPair<QString, QString> const generatorModelNames = QPair<QString, QString>(generatorModelName, generatorNormalizerModelName);

		if (!mMainWindowInterface->errorReporter()->wereErrors()) {
			if (QMessageBox::question(mMainWindowInterface->windowWidget()
									  , tr("loading.."), QString(tr("Do you want to load generated generator?")),
									  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
			loadNewEditor(generatorModelFullName, generatorModelNames, generatorModelId
						  , SettingsManager::value("pathToQmake", "").toString()
						  , SettingsManager::value("pathToMake", "").toString()
						  , SettingsManager::value("pluginExtension", "").toString()
						  , SettingsManager::value("prefix", "").toString());
		}
	}
}

void GeneratorsGeneratorPlugin::loadNewEditor(QString const &directoryName
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

	if (!mMainWindowInterface->unloadPlugin(pluginName)) {
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

			if (mMainWindowInterface->loadPlugin(prefix + pluginName + "." + extension, pluginName)) {
				progress->setValue(100);
			}
		}
	}

	if (progress->value() != 100) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot load new generator plugin"));
//		deleteGeneratedFiles(directoryName, normalizerPluginName); //qwerty_temp
	}
	progress->setValue(100);
	progress->close();
	delete progress;
}

void GeneratorsGeneratorPlugin::deleteGeneratedFiles(QString const &directoryName, QString const &fileBaseName)
{
	QFile filePro(directoryName + "/" + fileBaseName + ".pro");
	filePro.remove();
}


