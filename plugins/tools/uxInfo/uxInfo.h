/* Copyright 2007-2015 QReal Research Group
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

#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

namespace uxInfo {

/// Singleton class for collecting user experience information about setting changes,
/// click coordinates, diagram element creation and others.
class UXInfo : public QObject
{
	Q_OBJECT

public:
	/// Returns the single instance of usability info collector.
	static UXInfo *instance();

	/// Add to ux files record about creation of new element on diagram.
	/// @param editorName name of the editor of the element.
	/// @param elementName name of the created element.
	static void reportCreation(QString const &editorName, QString const elementName);

	/// Add to ux files record about error of system after user actions.
	/// @param type type of the error (error/warning/critical/information).
	/// @param editorName name of the editor associated with this error.
	/// @param elementName name of the element associated with this error.
	/// @param message error message.
	static void reportErrors(QString const &type, QString const &editorName, QString const &elementName,
			QString const &message);

	/// Add to ux files record about total time of current session.
	/// @param totalTime total session time.
	static void reportTotalTime(QString const &totalTime);

	/// Add to ux files record about using menu elements.
	/// @param elementName name of menu element.
	/// @param status status (if any, i.e. for toggled actions).
	static void reportMenuElements(QString const &elementName, QString const &status = "none");

	/// Add to ux files record about mouse click coordinates.
	static void reportMouseClick(QPoint const &pos);

	/// Set status about collect or not collect ux information.
	static void setStatus(bool status);

	///Record about start of the usability test.
	static void reportTestStarted();

	///Record about finish of the usability test.
	static void reportTestFinished();

	/// Close all ux files.
	void closeUXInfo();

public slots:
	/// Records settings modification.
	/// @param name Settings name.
	/// @param oldValue old value of the settings.
	/// @param newValue new value of the settings.
	void reportSettingsChanges(QString const &name, QVariant const &oldValue, QVariant const &newValue);

private:
	UXInfo();
	~UXInfo();

	bool writeData(QTextStream const &stream);
	void reportCreationOfElements(QString const &editorName, QString const elementName);
	void reportErrorsOfElements(QString const &type, QString const &editorName, QString const &elementName,
			QString const &message);
	void reportTotalTimeOfExec(QString const &totalTime);
	void reportMenuElementsUsing(QString const &elementName, QString const &status = "none");
	void reportMouseClickPosition(QPoint const &pos);
	void reportSettingsChangesInfo(const QString &name, const QString &oldValue, const QString &newValue);
	void setActualStatus(bool status);
	void reportTestStartedInfo();
	void reportTestFinishedInfo();
	void closeUXFiles();
	QString currentDateTime();

	static UXInfo* object;
	bool mStatus;
	bool mNotEnoughDiskSpace;

	QFile mElementOnSceneCreationFile;
	QTextStream mElementOnSceneCreationStream;
	int mCreationNumber;

	QFile mErrorReporterFile;
	QTextStream mErrorReporterStream;
	int mErrorReporterNumber;

	QFile mTotalTimeFile;
	QTextStream mTotalTimeStream;

	QFile mMenuElementUsingFile;
	QTextStream mMenuElementUsingStream;
	int mMenuElementUsingNumber;

	QFile mMouseClickPositionFile;
	QTextStream mMouseClickPositionStream;
	int mMouseClickPositionNumber;

	QFile mSettingChangesFile;
	QTextStream mSettingChangesStream;
	int mSettingChangesNumber;

	int mTestNumber;
};
}
