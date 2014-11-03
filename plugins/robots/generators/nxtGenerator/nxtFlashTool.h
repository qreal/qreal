#pragma once

#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace nxtOsek {

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
	void uploadProgram(QFileInfo const &fileInfo);
	void runProgram(QFileInfo const &fileInfo);
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
