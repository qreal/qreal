/* Copyright 2017 QReal Research Group
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

#include "communicator/controllerCommunicator.h"

#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QTextCodec>

#include <qrkernel/settingsManager.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <pioneerKit/constants.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

using namespace pioneer;
using namespace pioneer::lua;
using namespace qReal;

ControllerCommunicator::ControllerCommunicator(
		qReal::ErrorReporterInterface * const errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface * const robotModelManager
		)
	: mUploadProcess(new QProcess)
	, mStartProcess(new QProcess)
	, mErrorReporter(errorReporter)
	, mRobotModelManager(robotModelManager)
{
	connect(mUploadProcess.data()
			, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
			, this
			, &ControllerCommunicator::onUploadCompleted);

	connect(mStartProcess.data()
			, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
			, this
			, &ControllerCommunicator::onStartCompleted);
}

ControllerCommunicator::~ControllerCommunicator()
{
	// Empty destructor to keep QScopedPointer happy.
}

void ControllerCommunicator::uploadProgram(const QFileInfo &program)
{
	QString pathToPython = SettingsManager::value(settings::pioneerPythonPath).toString();
	if (pathToPython.isEmpty()) {
		pathToPython = "python";
	}

#ifdef Q_OS_WIN
	const QString processName = QApplication::applicationDirPath() + "/pioneerUpload.bat";
#else
	const QString processName = QApplication::applicationDirPath() + "/pioneerUpload.sh";
#endif

	const QString pathToLuac = mRobotModelManager->model().name() == modelNames::realCopter
			? QApplication::applicationDirPath()
					+ "/"
					+ SettingsManager::value(settings::pioneerRealCopterLuaPath, "").toString()
			: SettingsManager::value(settings::pioneerSimulatorLuaPath, "").toString();

	const QString pathToControllerScript = QApplication::applicationDirPath() + "/";

	const QStringList addressList = address().split(' ', QString::SkipEmptyParts);
	QStringList args = QStringList({ program.absoluteFilePath() });
	args.append(addressList);
	args.append({ pathToLuac, pathToPython, pathToControllerScript });

	mUploadProcess->start(processName, args);

	mUploadProcess->waitForStarted();
	if (mUploadProcess->state() != QProcess::Running) {
		mErrorReporter->addError(tr("Unable to execute script"));
		QStringList errors = QString(mUploadProcess->readAllStandardError()).split("\n", QString::SkipEmptyParts);
		for (const auto &error : errors) {
			mErrorReporter->addInformation(error);
		}

		done();
	} else {
		mErrorReporter->addInformation(tr("Uploading started, please wait..."));
	}
}

void ControllerCommunicator::runProgram(const QFileInfo &program)
{
	mIsStartNeeded = true;
	uploadProgram(program);
}

void ControllerCommunicator::onUploadCompleted()
{
	QStringList errors = toUnicode(mUploadProcess->readAllStandardError()).split("\n", QString::SkipEmptyParts);
	errors << toUnicode(mUploadProcess->readAllStandardOutput()).split("\n", QString::SkipEmptyParts);
	for (const auto &error : errors) {
		mErrorReporter->addInformation(error);
	}

	mErrorReporter->addInformation(tr("Uploading finished."));

	if (mIsStartNeeded) {
		doRunProgram();
		mIsStartNeeded = false;
	} else {
		done();
	}
}

void ControllerCommunicator::onStartCompleted()
{
	QStringList errors = toUnicode(mStartProcess->readAllStandardError()).split("\n", QString::SkipEmptyParts);
	errors << toUnicode(mStartProcess->readAllStandardOutput()).split("\n", QString::SkipEmptyParts);
	for (const auto &error : errors) {
		mErrorReporter->addInformation(error);
	}

	mErrorReporter->addInformation(tr("Starting finished."));
	done();
}

void ControllerCommunicator::doRunProgram()
{
	QString pathToPython = SettingsManager::value(settings::pioneerPythonPath).toString();
	if (pathToPython.isEmpty()) {
		pathToPython = "python";
	}

#ifdef Q_OS_WIN
	const QString processName = QApplication::applicationDirPath() + "/pioneerStart.bat";
#else
	const QString processName = QApplication::applicationDirPath() + "/pioneerStart.sh";
#endif
	const QString pathToControllerScript = QApplication::applicationDirPath() + "/";

	const QStringList addressList = address().split(' ', QString::SkipEmptyParts);
	QStringList args = addressList;
	args.append({ pathToPython, pathToControllerScript });

	mStartProcess->start(processName, args);
	mStartProcess->waitForStarted();
	if (mStartProcess->state() != QProcess::Running) {
		mErrorReporter->addError(tr("Unable to execute script"));
		done();
	} else {
		mErrorReporter->addInformation(tr("Starting program, please wait..."));
	}
}

QString ControllerCommunicator::toUnicode(const QByteArray &str)
{
#ifdef WIN32
	QTextCodec *codec = QTextCodec::codecForName("cp866");
#else
	QTextCodec *codec = QTextCodec::codecForLocale();
#endif

	return codec ? codec->toUnicode(str) : QString(str);
}

QString ControllerCommunicator::address()
{
	const bool useComPort = SettingsManager::value(settings::pioneerUseComPort).toBool();

	const QString server = SettingsManager::value(settings::pioneerBaseStationIP).toString();
	if (!useComPort && server.isEmpty()) {
		mErrorReporter->addError(tr("Pioneer base station IP address is not set. It can be set in Settings window."));
		return "";
	}

	const QString port = SettingsManager::value(settings::pioneerBaseStationPort).toString();
	if (!useComPort && port.isEmpty()) {
		mErrorReporter->addError(tr("Pioneer base station port is not set. It can be set in Settings window."));
		return "";
	}

	const QString comPort = SettingsManager::value(settings::pioneerComPort).toString();

	if (useComPort && comPort.isEmpty()) {
		mErrorReporter->addError(tr("Pioneer COM port is not set. It can be set in Settings window."));
		return "";
	}

	return useComPort ? QString("--serial %1").arg(comPort) : QString("--address %1:%2").arg(server).arg(port);
}

void ControllerCommunicator::done()
{
	if (mIsStartNeeded) {
		emit runCompleted();
	} else {
		emit uploadCompleted();
	}
}
