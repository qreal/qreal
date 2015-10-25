#include "pluginLoader.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include <QtCore/QProcess>

using namespace qReal;

PluginLoader::PluginLoader(gui::MainWindowInterpretersInterface &interpretersInterface)
	: mMainWindowInterpretersInterface(interpretersInterface)
{
}


void PluginLoader::loadNewEditor(const QString &directoryName
		, const QString &pluginName
		, const QString &pluginId
		, const QString &commandFirst
		, const QString &commandSecond
		, const QString &extension
		, const QString &prefix
		, const QString &buildConfiguration)
{
	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(mMainWindowInterpretersInterface.windowWidget()
				, tr("error"), tr("please, fill compiler settings"));

		return;
	}

	const QString realPluginName = fullPluginName(pluginName, buildConfiguration, extension, prefix);
	QProgressBar *progress = progressBar();

	// TODO_CONSTRAINTS
	if (!mMainWindowInterpretersInterface.unloadConstraintsPlugin(realPluginName, pluginId)) {
		QMessageBox::warning(mMainWindowInterpretersInterface.windowWidget()
				, tr("error"), tr("cannot unload plugin"));
		//deleteGeneratedFiles(directoryName, normalizerPluginName);
		progress->close();
		delete progress;

		return;
	}

	progress->setValue(20);

	QProcess builder;
	builder.setEnvironment(QProcess::systemEnvironment());
	builder.setWorkingDirectory(directoryName);
	builder.start(commandFirst, {"CONFIG+=" + buildConfiguration});

	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
		progress->setValue(60);
		builder.start(commandSecond);

		if (builder.waitForFinished() && (builder.exitCode() == 0)) {
			progress->setValue(80);

			// TODO_CONSTRAINTS
			if (mMainWindowInterpretersInterface.loadConstraintsPlugin(realPluginName)) {
				progress->setValue(100);
			}
		}
	}

	if (progress->value() != 100) {
		QMessageBox::warning(mMainWindowInterpretersInterface.windowWidget(), tr("error")
				, tr("cannot load new constraints plugin"));
		//deleteGeneratedFiles(directoryName, normalizerPluginName);
	}

	progress->setValue(100);
	progress->close();
	delete progress;
}

QString PluginLoader::fullPluginName(const QString &pluginName
		, const QString &buildConfiguration
		, const QString &extension
		, const QString &prefix) const
{
	QString suffix = "";
	if (buildConfiguration == "debug") {
		suffix += "-d";
	}

	return prefix + pluginName + suffix + "." + extension;
}

QProgressBar *PluginLoader::progressBar() const
{
	const int progressBarWidth = 240;
	const int progressBarHeight = 20;

	QProgressBar *progress = new QProgressBar(mMainWindowInterpretersInterface.windowWidget());
	progress->show();

	QApplication::processEvents();

	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	return progress;
}

void PluginLoader::deleteGeneratedFiles(const QString &directoryName
		, const QString &pluginName)
{

}
