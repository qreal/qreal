#pragma once

#include <QtCore/QProcess>

#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace robots {
namespace generator {

/// Class that handles flashing NXT robot via USB.
class NxtFlashTool : public QObject
{
	Q_OBJECT

public:
	explicit NxtFlashTool(qReal::ErrorReporterInterface *errorReporter);

//signals:
//	void showErrors(gui::ErrorReporter * const errorReporter);
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

	qReal::ErrorReporterInterface *mErrorReporter;
	QProcess mFlashProcess;
	QProcess mUploadProcess;

	UploadState mUploadState;
};

}
}
