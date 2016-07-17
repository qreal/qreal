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

#include "uxInfo.h"

#include <QtCore/QDir>
#include <QtCore/QPointF>
#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

using namespace uxInfo;

UXInfo* UXInfo::object = nullptr;

QString const uxDir = "uxInfo";
QString const elementCreationFileName = "/elementOnSceneCreation.txt";
QString const errorReporterFileName = "/errorReporter.txt";
QString const totalTimeFileName = "/totalTime.txt";
QString const menuElementUsingFileName = "/menuElementUsing.txt";
QString const mouseClickPositionFileName = "/mouseClickPosition.txt";
QString const settingChangesFileName = "/settingChanges.txt";
QString const uxInfoDirName = "usabilityFiles";

UXInfo::UXInfo()
{
	if (qReal::SettingsManager::value("usabilityTestingMode").toBool()) {
		QString const dir = QApplication::applicationDirPath() + "/" + uxDir;
		if (!QDir(dir).exists()) {
			QDir(dir).mkpath(dir);
		}

		///create and open all files
		mElementOnSceneCreationFile.setFileName(dir + elementCreationFileName);
		if (mElementOnSceneCreationFile.open(QFile::WriteOnly | QFile::Truncate)) {
			mElementOnSceneCreationStream.setDevice(&mElementOnSceneCreationFile);
		}

		mCreationNumber = 1;

		mErrorReporterFile.setFileName(dir + errorReporterFileName);
		if (mErrorReporterFile.open(QFile::WriteOnly | QFile::Truncate)) {
			mErrorReporterStream.setDevice(&mErrorReporterFile);
		}

		mErrorReporterNumber = 1;

		mTotalTimeFile.setFileName(dir + totalTimeFileName);
		if (mTotalTimeFile.open(QFile::WriteOnly | QFile::Truncate)) {
			mTotalTimeStream.setDevice(&mTotalTimeFile);
		}

		mMenuElementUsingFile.setFileName(dir + menuElementUsingFileName);
		if (mMenuElementUsingFile.open(QFile::WriteOnly | QFile::Truncate)) {
			mMenuElementUsingStream.setDevice(&mMenuElementUsingFile);
		}

		mMenuElementUsingNumber = 1;

		mMouseClickPositionFile.setFileName(dir + mouseClickPositionFileName);
		if (mMouseClickPositionFile.open(QFile::WriteOnly | QFile::Truncate)) {
			mMouseClickPositionStream.setDevice(&mMouseClickPositionFile);
		}

		mMouseClickPositionNumber = 1;

		mSettingChangesFile.setFileName(dir + settingChangesFileName);
		if (mSettingChangesFile.open(QFile::WriteOnly | QFile::Truncate)) {
			mSettingChangesStream.setDevice(&mSettingChangesFile);
		}

		mSettingChangesNumber = 1;

		mTestNumber = 1;
	}
	mNotEnoughDiskSpace = false;
}

UXInfo::~UXInfo()
{
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
	if (!writeData(mElementOnSceneCreationStream)) {
		return;
	}

	mElementOnSceneCreationStream << mCreationNumber << " "
			<< editorName << " "
			<< elementName << " "
			<< currentDateTime() << "\n";

	mCreationNumber++;
}

void UXInfo::reportErrorsOfElements(const QString &type, const QString &editorName, const QString &elementName
		, const QString &message)
{
	if (!writeData(mErrorReporterStream)) {
		return;
	}

	mErrorReporterStream << mErrorReporterNumber << " "
			<< type << " "
			<< editorName << " "
			<< elementName << " "
			<< message << " "
			<< currentDateTime() << "\n";

	mErrorReporterNumber++;
}

void UXInfo::reportTotalTimeOfExec(QString const &totalTime)
{
	if (!writeData(mTotalTimeStream)) {
		return;
	}

	mTotalTimeStream << "TotalSessionTime: "
			<< totalTime << " secs Exit code:"
			<< 0 << "\n";
}

void UXInfo::reportMenuElementsUsing(const QString &elementName, const QString &status)
{
	if (!writeData(mMenuElementUsingStream)) {
		return;
	}

	QString const statusText = (status == "none") ? "" : status + " ";
	mMenuElementUsingStream << mMenuElementUsingNumber << " "
			<< elementName << " "
			<< statusText
			<< currentDateTime() << "\n";

	mMenuElementUsingNumber++;
}

void UXInfo::reportMouseClickPosition(const QPoint &pos)
{
	if (!writeData(mMouseClickPositionStream)) {
		return;
	}

	mMouseClickPositionStream << mMouseClickPositionNumber << " ("
			<< QString::number(pos.x()) << ", "
			<< QString::number(pos.y()) << ") "
			<< currentDateTime() << "\n";

	mMouseClickPositionNumber++;
}

void UXInfo::reportSettingsChangesInfo(const QString &name, const QString &oldValue, const QString &newValue)
{
	if (!writeData(mSettingChangesStream)) {
		return;
	}

	mSettingChangesStream << mSettingChangesNumber << " "
			<< name << " "
			<< oldValue << " "
			<< newValue << " "
			<< currentDateTime() << "\n";

	mSettingChangesNumber++;
}

void UXInfo::setActualStatus(bool status)
{
	mStatus = status;
}

void UXInfo::reportTestStartedInfo()
{
	if (!writeData(mSettingChangesStream)) {
		return;
	}

	QList<QTextStream *> streamList;
	streamList << &mElementOnSceneCreationStream << &mErrorReporterStream << &mTotalTimeStream
			<< &mMenuElementUsingStream << &mMouseClickPositionStream << &mSettingChangesStream;

	QString const now = currentDateTime();
	for (int i = 0; i < streamList.length(); ++i) {
		*(streamList[i]) << "Test " << mTestNumber << " started at " << now << "\n";
	}
}

void UXInfo::reportTestFinishedInfo()
{
	if (!writeData(mSettingChangesStream)) {
		return;
	}

	QList<QTextStream *> streamList;
	streamList << &mElementOnSceneCreationStream << &mErrorReporterStream << &mTotalTimeStream
			<< &mMenuElementUsingStream << &mMouseClickPositionStream << &mSettingChangesStream;

	QString const now = currentDateTime();
	for (int i = 0; i < streamList.length(); ++i) {
		*(streamList[i]) << "Test " << mTestNumber << " finished at " << now << "\n";
	}
	mTestNumber++;
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
	if (object == nullptr) {
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

void UXInfo::reportCreation(const QString &editorName, const QString elementName)
{
	instance()->reportCreationOfElements(editorName, elementName);
}

void UXInfo::reportErrors(const QString &type, const QString &editorName, const QString &elementName,
		const QString &message)
{
	instance()->reportErrorsOfElements(type, editorName, elementName, message);
}

void UXInfo::reportTotalTime(QString const &totalTime)
{
	instance()->reportTotalTimeOfExec(totalTime);
}

void UXInfo::reportMenuElements(const QString &elementName, const QString &status)
{
	instance()->reportMenuElementsUsing(elementName, status);
}

void UXInfo::reportMouseClick(const QPoint &pos)
{
	instance()->reportMouseClickPosition(pos);
}

void UXInfo::reportSettingsChanges(QString const &name, QVariant const &oldValue, QVariant const &newValue)
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
