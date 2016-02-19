#include "trikKitInterpreterCommon/trikQtsInterpreter.h"

trik::TrikQtsInterpreter::TrikQtsInterpreter(
        const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model
        ) : mBrick(model), mScriptRunner(mBrick, nullptr, nullptr)
{
}

void trik::TrikQtsInterpreter::interpretStringScript(const QString &script)
{
	mScriptRunner.runDirectCommand(script);
}

void trik::TrikQtsInterpreter::abort()
{
	mScriptRunner.abort();
}
