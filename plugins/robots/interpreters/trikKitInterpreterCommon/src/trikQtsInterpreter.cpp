#include "trikKitInterpreterCommon/trikQtsInterpreter.h"

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include <QDebug>

QScriptValue printRedirect(QScriptContext * context, QScriptEngine * engine)
{
	QString result;
	for (int i = 0; i < context->argumentCount(); ++i) {
		if (i > 0) {
			result.append(" ");
		}

		result.append(context->argument(i).toString());
	}

	engine->evaluate(QString("brick.log(\"%1\");").arg(result));

	return engine->toScriptValue(result);
}

trik::TrikQtsInterpreter::TrikQtsInterpreter(
        const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model
		) : mRunning(false), mBrick(model), mScriptRunner(mBrick, nullptr, nullptr), mErrorReporter(nullptr)
{
	connect(&mBrick, &TrikBrick::error, this, &TrikQtsInterpreter::reportError);
	mScriptRunner.registerUserFunction("print", printRedirect);
//	connect(&mBrick, &TrikBrick::log, [this](const QString &msg){
//		QMetaObject::invokeMethod(this, "reportLog", Q_ARG(const QString &, msg));
//	});
//	auto redirectPrint = [](QScriptContext * ctx, QScriptEngine * eng) -> QScriptValue {
//		const auto txt = ctx->argument(0).toString();
//		eng->globalObject().property("brick").
//		return QScriptValue();
//	};
	mScriptRunner.registerUserFunction("print", printRedirect);
	connect(&mScriptRunner, SIGNAL(completed(QString,int)), this, SLOT(scriptFinished(QString,int)));
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
	mRunning = true;
	mScriptRunner.run(script);
}

void trik::TrikQtsInterpreter::abort()
{
	//mScriptRunner.abort();
	QMetaObject::invokeMethod(&mScriptRunner, "abort"); // just a wild test
	mRunning = false; // reset brick?
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

void trik::TrikQtsInterpreter::scriptFinished(const QString &error, int scriptId)
{
	Q_UNUSED(scriptId);
	if (!error.isEmpty()) {
		reportError(error);
	}
	if (mRunning) { /// @todo: figure out better place for this check - it should avoid double aborts
		emit completed();
	}
}
