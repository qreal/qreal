#pragma once

#include <QtCore/QProcess>
#include "errorReporter.h"

namespace qReal {
namespace gui {

/// Class that handles flashing NXT robot via USB. Should be moved to Robots plugin in new architecture

class NxtFlashTool : public QObject
{
	Q_OBJECT

public:
	explicit NxtFlashTool(ErrorReporter *errorReporter);

signals:
	void showErrors(gui::ErrorReporter * const errorReporter);

public slots:
	void flashRobot();
	void uploadProgram();

	void error(QProcess::ProcessError error);
	void readNxtFlashData();
	void nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void readNxtUploadData();
	void nxtUploadingFinished(int exitCode, QProcess::ExitStatus exitStatus);


private:
	enum UploadState {
		clean,
		compile,
		link,
		uploadStart,
		flash,
		done
	};

	ErrorReporter *mErrorReporter;
	QProcess mFlashProcess;
	QProcess mUploadProcess;

	UploadState mUploadState;
};

}
}
