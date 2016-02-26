#include "trikKitInterpreterCommon/trikQtsInterpreter.h"

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

trik::TrikQtsInterpreter::TrikQtsInterpreter(
        const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model
        ) : mBrick(model), mScriptRunner(mBrick, nullptr, nullptr), mErrorReporter(nullptr)
{
	connect(&mBrick, TrikBrick::error, this, trik::TrikQtsInterpreter::reportError);
}

void trik::TrikQtsInterpreter::interpretStringScript(const QString &script)
{
	mScriptRunner.runDirectCommand(script);
}

void trik::TrikQtsInterpreter::abort()
{
	mScriptRunner.abort();
}

void trik::TrikQtsInterpreter::init()
{
	mBrick.init();
}

void trik::TrikQtsInterpreter::setErrorReporter(qReal::ErrorReporterInterface &errorReporter)
{
	mErrorReporter = &errorReporter;
}

void trik::TrikQtsInterpreter::reportError(const QString &msg)
{
	mErrorReporter->addError(msg);
//	mBrick.abort(); what if there are more errors?
}
