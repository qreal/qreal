/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <time.h>

#include <QtCore/QtPlugin>
#include <QtCore/QTranslator>
#include <QtCore/QDirIterator>
#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

#include <qrgui/mainWindow/mainWindow.h>
#include <qrgui/thirdparty/windowsmodernstyle.h>

#include <qrgui/mainWindow/qrealApplication.h>

using namespace qReal;

const int maxLogSize = 10 * 1024 * 1024;  // 10 MB

void clearConfig()
{
	SettingsManager::clearSettings();
	SettingsManager::instance()->saveData();
}

void loadTranslators(const QString &locale)
{
	QDir translationsDirectory(PlatformInfo::applicationDirPath() + "/translations/" + locale);
	QDirIterator directories(translationsDirectory, QDirIterator::Subdirectories);
	while (directories.hasNext()) {
		for (const QFileInfo &translatorFile : QDir(directories.next()).entryInfoList(QDir::Files)) {
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

void setNativeDialogs(bool disabled)
{
	QApplication::instance()->setProperty("DontUseNativeDialogs", QVariant(disabled));
}

void initLogging()
{
	const QDir logsDir(PlatformInfo::applicationDirPath() + "/logs");
	if (logsDir.mkpath(logsDir.absolutePath())) {
		Logger::addLogTarget(logsDir.filePath("qreal.log"), maxLogSize, 2, QsLogging::DebugLevel);
		Logger::addLogTarget(logsDir.filePath("actions.log"), maxLogSize, 2, QsLogging::TraceLevel);
	}
}

MainWindow* start()
{
	QCoreApplication *coreApp = QApplication::instance();
	QApplication *aApp = qobject_cast<QApplication *> (coreApp);
	QRealApplication *app = qobject_cast<QRealApplication *> (aApp);
	if (!app) {
		return nullptr;
	}

	qsrand(time(0));
	// setDefaultLocale(app->arguments().contains("--no-locale"));
	// commented out because of the bug. FIXME: a bug with QTest::keyClick for no askii characters
	setDefaultLocale(true);
	setNativeDialogs(true);

	initLogging();
	QLOG_INFO() << "------------------- APPLICATION STARTED --------------------";
	QLOG_INFO() << "Running on" << PlatformInfo::prettyOsVersion();
	QLOG_INFO() << "Arguments:" << app->arguments();
	QLOG_INFO() << "Setting default locale to" << QLocale().name();

	QString fileToOpen;
	if (app->arguments().count() > 1) {
		if (app->arguments().contains("--clear-conf")) {
			clearConfig();
			return 0;
		} else {
			const int setIndex = app->arguments().indexOf("--config");
			if (setIndex > -1) {
				const QString settingsFileName = app->arguments().at(setIndex + 1);
				SettingsManager::instance()->loadSettings(settingsFileName);
			}

			for (const QString &argument : app->arguments()) {
				if (argument.endsWith(".qrs") || argument.endsWith(".qrs'") || argument.endsWith(".qrs\"")) {
					fileToOpen = argument;
					break;
				}
			}
		}
	}

	#ifndef NO_STYLE_WINDOWSMODERN
		app->setStyle(new WindowsModernStyle());
	#endif

	MainWindow *window = new MainWindow(fileToOpen);
	SettingsManager::setValue("scriptInterpretation", true);
	if (!window->isVisible()) {
		return nullptr;
	}

	return window;
}
