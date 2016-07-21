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

#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace nxt {

/// Class that handles flashing NXT robot via USB.
class NxtFlashTool : public QObject
{
	Q_OBJECT

public:
	enum RunPolicy {
		Ask = 0
		, AlwaysRun
		, NeverRun
	};

	explicit NxtFlashTool(qReal::ErrorReporterInterface *errorReporter);

public slots:
	void flashRobot();
	void uploadProgram(const QFileInfo &fileInfo);
	void runProgram(const QFileInfo &fileInfo);
	void runLastProgram();

	void error(QProcess::ProcessError error);
	void readNxtFlashData();
	bool askToRun(QWidget *parent);
	void nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void readNxtUploadData();
	void nxtUploadingFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
	void flashingComplete(bool success);
	void uploadingComplete(bool success);

private:
	enum UploadState {
		clean,
		compile,
		compilationError,
		link,
		uploadStart,
		flash,
		done
	};

	const QString path(const QString &file = QString()) const;

	qReal::ErrorReporterInterface *mErrorReporter;
	QProcess mFlashProcess;
	QProcess mUploadProcess;
	QProcess mRunProcess;

	bool mIsFlashing;
	bool mIsUploading;

	QFileInfo mSource;

	UploadState mUploadState;
};

}
