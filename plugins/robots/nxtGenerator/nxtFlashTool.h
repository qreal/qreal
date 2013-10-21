#pragma once

#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace robots {
namespace generators {

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
	void uploadProgram(QFileInfo const &fileInfo);

	void error(QProcess::ProcessError error);
	void readNxtFlashData();
	void nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void readNxtUploadData();
	void nxtUploadingFinished(int exitCode, QProcess::ExitStatus exitStatus);


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

	UploadState mUploadState;
};

}
}
}
