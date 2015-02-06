#include <time.h>

#include <QtCore/QtPlugin>
#include <QtCore/QTranslator>
#include <QtCore/QDirIterator>
#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

#include "mainWindow/mainWindow.h"
#include "thirdparty/windowsmodernstyle.h"

#include "qrealApplication.h"

using namespace qReal;

const int maxLogSize = 10 * 1024 * 1024;  // 10 MB

void clearConfig()
{
	SettingsManager::clearSettings();
	SettingsManager::instance()->saveData();
}

void loadTranslators(const QString &locale)
{
	QDir translationsDirectory(QApplication::applicationDirPath() + "/translations/" + locale);
	QDirIterator directories(translationsDirectory, QDirIterator::Subdirectories);
	while (directories.hasNext()) {
		for (QFileInfo const &translatorFile : QDir(directories.next()).entryInfoList(QDir::Files)) {
			QTranslator *translator = new QTranslator(qApp);
			translator->load(translatorFile.absoluteFilePath());
			QApplication::installTranslator(translator);
		}
	}
}

void setDefaultLocale(bool localizationDisabled)
{
	if (localizationDisabled) {
		QLocale::setDefault(QLocale::English);
		return;
	}

	const QString localeInSettings = SettingsManager::value("systemLocale").toString();
	const QString locale = localeInSettings.isEmpty() ? QLocale().name().left(2) : localeInSettings;
	if (!locale.isEmpty()) {
		QLocale::setDefault(QLocale(locale));
		loadTranslators(locale);
	}
}

void initLogging()
{
	QDir const logsDir(QApplication::applicationDirPath() + "/logs");
	if (logsDir.mkpath(logsDir.absolutePath())) {
		Logger::addLogTarget(logsDir.filePath("qreal.log"), maxLogSize, 2, QsLogging::DebugLevel);
		Logger::addLogTarget(logsDir.filePath("actions.log"), maxLogSize, 2, QsLogging::TraceLevel);
	}
}

int main(int argc, char *argv[])
{
	QRealApplication app(argc, argv);

	qsrand(time(0));
	setDefaultLocale(app.arguments().contains("--no-locale"));

	initLogging();
	QLOG_INFO() << "------------------- APPLICATION STARTED --------------------";
	QLOG_INFO() << "Running on" << PlatformInfo::prettyOsVersion();
	QLOG_INFO() << "Arguments:" << app.arguments();
	QLOG_INFO() << "Setting default locale to" << QLocale().name();

	QString fileToOpen;
	if (app.arguments().count() > 1) {
		if (app.arguments().contains("--clear-conf")) {
			clearConfig();
			return 0;
		} else {
			const int setIndex = app.arguments().indexOf("--config");
			if (setIndex > -1) {
				const QString settingsFileName = app.arguments().at(setIndex + 1);
				SettingsManager::instance()->loadSettings(settingsFileName);
			}

			for (const QString &argument : app.arguments()) {
				if (argument.endsWith(".qrs") || argument.endsWith(".qrs'") || argument.endsWith(".qrs\"")) {
					fileToOpen = argument;
					break;
				}
			}
		}
	}

#ifndef NO_STYLE_WINDOWSMODERN
	app.setStyle(new WindowsModernStyle());
#endif

	MainWindow window(fileToOpen);
	int exitCode = 0; // The window decided to not show itself, exiting now.

	if (window.isVisible()) {
		exitCode = app.exec();
	}

	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	return exitCode;
}
