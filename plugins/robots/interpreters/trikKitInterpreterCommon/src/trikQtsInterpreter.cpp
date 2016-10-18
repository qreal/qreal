#include "trikKitInterpreterCommon/trikQtsInterpreter.h"

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include <QDebug>

trik::TrikQtsInterpreter::TrikQtsInterpreter(
        const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model
		) : mRunning(false), mBrick(model), mScriptRunner(mBrick, nullptr, nullptr), mErrorReporter(nullptr)
{
	connect(&mBrick, &TrikBrick::error, this, &TrikQtsInterpreter::reportError);
//	connect(&mBrick, &TrikBrick::log, [this](const QString &msg){
//		QMetaObject::invokeMethod(this, "reportLog", Q_ARG(const QString &, msg));
//	});
//	auto redirectPrint = [](QScriptContext * ctx, QScriptEngine * eng) -> QScriptValue {
//		const auto txt = ctx->argument(0).toString();
//		eng->globalObject().property("brick").
//		return QScriptValue();
//	};
//	mScriptRunner.registerUserFunction("print", redirectPrint);
}

trik::TrikQtsInterpreter::~TrikQtsInterpreter()
{
	abort();
}

void trik::TrikQtsInterpreter::interpretCommand(const QString &script)
{
	mScriptRunner.runDirectCommand(script);
}

void trik::TrikQtsInterpreter::interpretScript(const QString &script)
{
	mScriptRunner.run(script);
	mRunning = true;
}

void trik::TrikQtsInterpreter::abort()
{
	mScriptRunner.abort();
	mRunning = false;
}

void trik::TrikQtsInterpreter::init()
{
	mBrick.init();
}

void trik::TrikQtsInterpreter::setErrorReporter(qReal::ErrorReporterInterface &errorReporter)
{
	mErrorReporter = &errorReporter;
}

bool trik::TrikQtsInterpreter::isRunning() const
{
	return mRunning;
}

void trik::TrikQtsInterpreter::reportError(const QString &msg)
{
	mErrorReporter->addError(msg);
//	mBrick.abort(); what if there are more errors?
}

void trik::TrikQtsInterpreter::reportLog(const QString &msg)
{
	mErrorReporter->addInformation("log: " + msg);
}

void trik::TrikQtsInterpreter::setRunning(bool running)
{
	mRunning = running;
}
