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

	void readNxtFlashData();
	void error(QProcess::ProcessError error);
	void nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	ErrorReporter *mErrorReporter;
	QProcess mFlashProcess;
};

}
}
