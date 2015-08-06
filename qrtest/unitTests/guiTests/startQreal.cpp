/* Copyright 2015 QReal Research Group
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

#include "./qrgui/mainWindow/mainWindow.h"
#include "./qrgui/thirdparty/windowsmodernstyle.h"

#include "./qrgui/mainWindow/qrealApplication.h"

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

void initLogging() // с логами возможна проблема, так как qapp;
{
	const QDir logsDir(QApplication::applicationDirPath() + "/logs");
	if (logsDir.mkpath(logsDir.absolutePath())) {
		Logger::addLogTarget(logsDir.filePath("qreal.log"), maxLogSize, 2, QsLogging::DebugLevel);
		Logger::addLogTarget(logsDir.filePath("actions.log"), maxLogSize, 2, QsLogging::TraceLevel);
	}
}

MainWindow* start()
{
	QCoreApplication *coreApp = QApplication::instance();
	QApplication *app = qobject_cast<QApplication *> (coreApp);
//	// может код ниже вынести отдельно? (который связан с instance())
//	QRealApplication *app = qobject_cast<QRealApplication *> (aApp);
	if (!app) {
		return nullptr;
	}
//	if (!app) {
//		qDebug() << aApp->arguments().count() << "\n" << aApp->arguments().join("\n");
//		int argc = aApp->arguments().count();
//		QStringList argv(aApp->arguments());
//		aApp->~QCoreApplication();
//		char *argChars[argc];
//		int i = 0;
//		foreach(QString argument, argv)
//		{
//			argChars[i] = new char[argument.toLocal8Bit().size() + 1];
//			strcpy(argChars[i], argument.toLocal8Bit().constData()); // chcp 866, мб toUtf8?
//			argChars[strlen(argChars[i])] = '\0';
//			++i;
//		}
//		app = new QRealApplication(argc, argChars);
//		for (; i > 0; --i) {
//			delete []argChars[i - 1];
//		}
//		qDebug() << app->arguments().count() << "\n" << app->arguments().join("\n");
//	}

	qsrand(time(0));
	setDefaultLocale(app->arguments().contains("--no-locale"));

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

	if (!window->isVisible()) {
		return nullptr;
	}
	return window;
}
