#pragma once

#include <QSharedPointer>

#include "robotModel/twoD/trikTwoDRobotModel.h"
#include "trikKitInterpreterCommon/trikbrick.h"
#include "trikScriptRunner/trikScriptRunner.h"

#include "declSpec.h"

namespace trik {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT TrikQtsInterpreter
		: public QObject
{
	Q_OBJECT

public:
	TrikQtsInterpreter(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model);
	~TrikQtsInterpreter() override;

	void interpretCommand(const QString &script);
	void interpretScript(const QString &script);
	void abort();

	void init();

	void setErrorReporter(qReal::ErrorReporterInterface &errorReporter);

	bool isRunning() const;

	void setRunning(bool running);

private slots:
	void reportError(const QString &msg);

private:
	//QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;
	bool mRunning;

	TrikBrick mBrick;
	trikScriptRunner::TrikScriptRunner mScriptRunner;
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
