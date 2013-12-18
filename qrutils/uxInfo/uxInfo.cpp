#include <QtCore/QDir>
#include <QtCore/QPointF>

#include "uxInfo.h"

using namespace utils;

UXInfo* UXInfo::object = NULL;

QTextStream UXInfo::elementOnSceneCreationStream;
QTextStream UXInfo::errorReporterStream;
QTextStream UXInfo::totalTimeStream;
QTextStream UXInfo::menuElementUsingStream;
QTextStream UXInfo::mouseClickPositionStream;
QTextStream UXInfo::settingChangesStream;

int UXInfo::creationNumber;
int UXInfo::errorReporterNumber;
int UXInfo::menuElementUsingNumber;
int UXInfo::mouseClickPositionNumber;
int UXInfo::settingChangesNumber;
int UXInfo::testNumber;

QString const elementCreationFileName = "/elementOnSceneCreation.txt";
QString const errorReporterFileName = "/errorReporter.txt";
QString const totalTimeFileName = "/totalTime.txt";
QString const menuElementUsingFileName = "/menuElementUsing.txt";
QString const mouseClickPositionFileName = "/mouseClickPosition.txt";
QString const settingChangesFileName = "/settingChanges.txt";
QString const uxInfoDirName = "usabilityFiles";

UXInfo::UXInfo()
{
	QDir dir(".");

	///create and open all files
	mElementOnSceneCreationFile.setFileName(dir.absolutePath() + elementCreationFileName);
	if (mElementOnSceneCreationFile.open(QFile::WriteOnly | QFile::Truncate)) {
		elementOnSceneCreationStream.setDevice(&mElementOnSceneCreationFile);
	}

	creationNumber = 1;

	mErrorReporterFile.setFileName(dir.absolutePath() + errorReporterFileName);
	if (mErrorReporterFile.open(QFile::WriteOnly | QFile::Truncate)) {
		errorReporterStream.setDevice(&mErrorReporterFile);
	}

	errorReporterNumber = 1;

	mTotalTimeFile.setFileName(dir.absolutePath() + totalTimeFileName);
	if (mTotalTimeFile.open(QFile::WriteOnly | QFile::Truncate)) {
		totalTimeStream.setDevice(&mTotalTimeFile);
	}

	mMenuElementUsingFile.setFileName(dir.absolutePath() + menuElementUsingFileName);
	if (mMenuElementUsingFile.open(QFile::WriteOnly | QFile::Truncate)) {
		menuElementUsingStream.setDevice(&mMenuElementUsingFile);
	}

	menuElementUsingNumber = 1;

	mMouseClickPositionFile.setFileName(dir.absolutePath() + mouseClickPositionFileName);
	if (mMouseClickPositionFile.open(QFile::WriteOnly | QFile::Truncate)) {
		mouseClickPositionStream.setDevice(&mMouseClickPositionFile);
	}

	mouseClickPositionNumber = 1;

	mSettingChangesFile.setFileName(dir.absolutePath() + settingChangesFileName);
	if (mSettingChangesFile.open(QFile::WriteOnly | QFile::Truncate)) {
		settingChangesStream.setDevice(&mSettingChangesFile);
	}

	settingChangesNumber = 1;

	testNumber = 1;
	mNotEnoughDiskSpace = false;
}

bool UXInfo::writeData(QTextStream const &stream)
{
	if (!mStatus || mNotEnoughDiskSpace) {
		return false;
	}

	if (stream.status() == QTextStream::WriteFailed) {
		mNotEnoughDiskSpace = true;
		return false;
	}
	return true;
}

void UXInfo::reportCreationOfElements(const QString &editorName, const QString elementName)
{
	if (!writeData(elementOnSceneCreationStream)) {
		return;
	}

	elementOnSceneCreationStream << creationNumber << " "
			<< editorName << " "
			<< elementName << " "
			<< currentDateTime() << "\n";
	creationNumber++;
}

void UXInfo::reportErrorsOfElements(const QString &type, const QString &editorName, const QString &elementName, const QString &message)
{
	if (!writeData(errorReporterStream)) {
		return;
	}

	errorReporterStream << errorReporterNumber << " "
			<< type << " "
			<< editorName << " "
			<< elementName << " "
			<< message << " "
			<< currentDateTime() << "\n";
	errorReporterNumber++;
}

void UXInfo::reportTotalTimeOfExec(const QString &totalTime, const int &exitCode)
{
	if (!writeData(totalTimeStream)) {
		return;
	}

	totalTimeStream << "TotalSessionTime: "
			<< totalTime << " msecs Exit code:"
			<< exitCode << "\n";
}

void UXInfo::reportMenuElementsUsing(const QString &elementName, const QString &status)
{
	if (!writeData(menuElementUsingStream)) {
		return;
	}

	QString const statusText = (status == "none") ? "" : status + " ";
	menuElementUsingStream << menuElementUsingNumber << " "
			<< elementName << " "
			<< statusText
			<< currentDateTime() << "\n";
	menuElementUsingNumber++;
}

void UXInfo::reportMouseClickPosition(const QPoint &pos)
{
	if (!writeData(mouseClickPositionStream)) {
		return;
	}

	mouseClickPositionStream << mouseClickPositionNumber << " ("
			<< QString::number(pos.x()) << ", "
			<< QString::number(pos.y()) << ") "
			<< currentDateTime() << "\n";
	mouseClickPositionNumber++;
}

void UXInfo::reportSettingsChangesInfo(const QString &name, const QString &oldValue, const QString &newValue)
{
	if (!writeData(settingChangesStream)) {
		return;
	}

	settingChangesStream << settingChangesNumber << " "
			<< name << " "
			<< oldValue << " "
			<< newValue << " "
			<< currentDateTime() << "\n";
	settingChangesNumber++;
}

void UXInfo::setActualStatus(bool status)
{
	mStatus = status;
}

void UXInfo::reportTestStartedInfo()
{
	if (!writeData(settingChangesStream)) {
		return;
	}

	QList<QTextStream *> streamList;
	streamList << &elementOnSceneCreationStream << &errorReporterStream << &totalTimeStream
			<< &menuElementUsingStream << &mouseClickPositionStream << &settingChangesStream;
	QString const now = currentDateTime();
	for (int i = 0; i < streamList.length(); ++i) {
		*(streamList[i]) << "Test " << testNumber << " started at " << now << "\n";
	}
}

void UXInfo::reportTestFinishedInfo()
{
	if (!writeData(settingChangesStream)) {
		return;
	}

	QList<QTextStream *> streamList;
	streamList << &elementOnSceneCreationStream << &errorReporterStream << &totalTimeStream
			<< &menuElementUsingStream << &mouseClickPositionStream << &settingChangesStream;
	QString const now = currentDateTime();
	for (int i = 0; i < streamList.length(); ++i) {
		*(streamList[i]) << "Test " << testNumber << " finished at " << now << "\n";
	}
	testNumber++;
}

void UXInfo::closeUXFiles()
{
	mElementOnSceneCreationFile.close();
	mErrorReporterFile.close();
	mTotalTimeFile.close();
	mMenuElementUsingFile.close();
	mMouseClickPositionFile.close();
	mSettingChangesFile.close();
}

QString UXInfo::currentDateTime()
{
	return QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz");
}

UXInfo *UXInfo::instance()
{
	if (object == NULL) {
		object = new UXInfo();
	}
	return object;
}

void UXInfo::closeUXInfo()
{
	closeUXFiles();

	if (!mStatus) {
		return;
	}

	QDir dir(".");
	QString const newDirName = currentDateTime();
	QString const oldElementOnSceneCreationName = mElementOnSceneCreationFile.fileName();
	QString const oldErrorReporterName = mErrorReporterFile.fileName();
	QString const oldTotalTimeName = mTotalTimeFile.fileName();
	QString const oldMenuElementUsingName = mMenuElementUsingFile.fileName();
	QString const oldMouseClickPositionName = mMouseClickPositionFile.fileName();
	QString const oldSettingChangesName = mSettingChangesFile.fileName();

	QString const newFileElementOnSceneCreationName = newDirName + elementCreationFileName;
	QString const newFileErrorReporterName = newDirName + errorReporterFileName;
	QString const newFileTotalTimeName = newDirName + totalTimeFileName;
	QString const newFileMenuElementUsingName = newDirName + menuElementUsingFileName;
	QString const newFileMouseClickPositionName = newDirName + mouseClickPositionFileName;
	QString const newFileSettingChangesName = newDirName + settingChangesFileName;

	if (dir.cdUp()) {
		if (!dir.exists(uxInfoDirName)) {
			dir.mkdir(uxInfoDirName);
		}
		dir.cd(uxInfoDirName);
		QString const dirAbsolutePathName = dir.absolutePath() + "/";
		QString const newElementOnSceneCreationName = dirAbsolutePathName + newFileElementOnSceneCreationName;
		QString const newErrorReporterName = dirAbsolutePathName + newFileErrorReporterName;
		QString const newTotalTimeName = dirAbsolutePathName + newFileTotalTimeName;
		QString const newMenuElementUsingName = dirAbsolutePathName + newFileMenuElementUsingName;
		QString const newMouseClickPositionName = dirAbsolutePathName + newFileMouseClickPositionName;
		QString const newSettingChangesName = dirAbsolutePathName + newFileSettingChangesName;

		dir.mkdir(newDirName);
		QFile::copy(oldElementOnSceneCreationName, newElementOnSceneCreationName);
		QFile::copy(oldErrorReporterName, newErrorReporterName);
		QFile::copy(oldTotalTimeName, newTotalTimeName);
		QFile::copy(oldMenuElementUsingName, newMenuElementUsingName);
		QFile::copy(oldMouseClickPositionName, newMouseClickPositionName);
		QFile::copy(oldSettingChangesName, newSettingChangesName);
	}
}

void UXInfo::reportCreation(const QString &editorName, const QString elementName)
{
	instance()->reportCreationOfElements(editorName, elementName);
}

void UXInfo::reportErrors(const QString &type, const QString &editorName, const QString &elementName,
		const QString &message)
{
	instance()->reportErrorsOfElements(type, editorName, elementName, message);
}

void UXInfo::reportTotalTime(QString const &totalTime, int const &exitCode)
{
	instance()->reportTotalTimeOfExec(totalTime, exitCode);
}

void UXInfo::reportMenuElements(const QString &elementName, const QString &status)
{
	instance()->reportMenuElementsUsing(elementName, status);
}

void UXInfo::reportMouseClick(const QPoint &pos)
{
	instance()->reportMouseClickPosition(pos);
}

void UXInfo::reportSettingsChanges(const QString &name, const QVariant &oldValue, const QVariant &newValue)
{
	instance()->reportSettingsChangesInfo(name, oldValue.toString(), newValue.toString());
}

void UXInfo::setStatus(bool status)
{
	instance()->setActualStatus(status);
}

void UXInfo::reportTestStarted()
{
	instance()->reportTestStartedInfo();
}

void UXInfo::reportTestFinished()
{
	instance()->reportTestFinishedInfo();
}

UXInfo::~UXInfo()
{
}
