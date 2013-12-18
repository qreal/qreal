#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include <qrutils/utilsDeclSpec.h>
#include <qrkernel/uxInfoInterface.h>

namespace utils {

/// Singleton class (realization of UXInfoInterface) for collecting
/// user experience information about setting changes,
///click coordinates, diagram element creation and others.
class QRUTILS_EXPORT UXInfo : public qReal::UXInfoInterface {

public:
	static UXInfo* instance();
	~UXInfo();

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
	/// @param exitCode exit code of application.
	static void reportTotalTime(QString const &totalTime, int const &exitCode);

	/// Add to ux files record about using menu elements.
	/// @param elementName name of menu element.
	/// @param status status (if any, i.e. for toggled actions).
	static void reportMenuElements(QString const &elementName, QString const &status = "none");

	/// Add to ux files record about mouse click coordinates.
	static void reportMouseClick(QPoint const &pos);

	/// Recording change of settings.
	/// @param name Settings name.
	/// @param oldValue old value of the settings.
	/// @param newValue new value of the settings.
	void reportSettingsChanges(QString const &name, QVariant const &oldValue, QVariant const &newValue);

	/// Set status about collect or not collect ux information.
	static void setStatus(bool status);

	///Record about start of the usability test.
	static void reportTestStarted();

	///Record about finish of the usability test.
	static void reportTestFinished();

	/// Close all ux files.
	void closeUXInfo();

private:
	UXInfo();

	bool writeData(QTextStream const &stream);
	void reportCreationOfElements(QString const &editorName, QString const elementName);
	void reportErrorsOfElements(QString const &type, QString const &editorName, QString const &elementName,
			QString const &message);
	void reportTotalTimeOfExec(QString const &totalTime, int const &exitCode);
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
