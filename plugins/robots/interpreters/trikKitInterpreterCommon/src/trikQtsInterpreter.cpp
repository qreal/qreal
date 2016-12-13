#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>

#include "trikKitInterpreterCommon/trikQtsInterpreter.h"

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include <twoDModel/engine/model/timeline.h>

#include <QDebug>

//QScriptValue printRedirect(QScriptContext * context, QScriptEngine * engine)
//{
//	QString result;
//	for (int i = 0; i < context->argumentCount(); ++i) {
//		if (i > 0) {
//			result.append(" ");
//		}

//		result.append(context->argument(i).toString());
//	}
//	if (!engine->isEvaluating())
//		qDebug("stoped evaluating!");
//	//bad: could sigsegv if execution aborted before executed.
//	engine->evaluate(QString("brick.log(\"%1\");").arg(result));

//	return engine->toScriptValue(result);
//}

const QString overrides = "script.random = brick.random;script.wait = brick.wait;script.time = brick.time;\n"
		"print = function() {var res = '';\n"
		"for(var i = 0; i < arguments.length; i++) {res += arguments[i].toString();}\n"
		"brick.log(res);return res;};";

trik::TrikQtsInterpreter::TrikQtsInterpreter(
        const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model
		) : mRunning(false), mBrick(model), mScriptRunner(mBrick, nullptr, nullptr), mErrorReporter(nullptr)
{
	connect(&mBrick, &TrikBrick::error, this, &TrikQtsInterpreter::reportError);
//	mScriptRunner.registerUserFunction("print", printRedirect);
//	connect(&mBrick, &TrikBrick::log, [this](const QString &msg){
//		QMetaObject::invokeMethod(this, "reportLog", Q_ARG(const QString &, msg));
//	});
//	auto redirectPrint = [](QScriptContext * ctx, QScriptEngine * eng) -> QScriptValue {
//		const auto txt = ctx->argument(0).toString();
//		eng->globalObject().property("brick").
//		return QScriptValue();
//	};
	//auto &t = dynamic_cast<twoDModel::model::Timeline &>(model->timeline());
	//t.setImmediateMode(true);
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
	mScriptRunner.run(overrides + script);
}

void trik::TrikQtsInterpreter::interpretScriptExercise(const QString &script, const QString &inputs)
{
	mRunning = true;
	QString newScript = overrides + initInputs(inputs) + script;
	qDebug() << newScript;
	mScriptRunner.run(newScript);
}

void trik::TrikQtsInterpreter::abort()
{
	//mScriptRunner.abort();
	mBrick.stopWaiting();
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

QString trik::TrikQtsInterpreter::initInputs(const QString &inputs) const
{
	auto jsValToStr = [this](const QJsonValue &val) -> QString {
		switch (val.type()) {
		case QJsonValue::Bool:
			return val.toBool() ? "true" : "false";
		case QJsonValue::Double:
			return QString::number(val.Double);//maybe increase precision
		case QJsonValue::String:
			return QString("\"%1\"").arg(val.toString());
		case QJsonValue::Array:
			return QString(QJsonDocument(val.toArray()).toJson(QJsonDocument::Compact));
		case QJsonValue::Object:
			return QString(QJsonDocument(val.toObject()).toJson(QJsonDocument::Compact));
		default:
			mErrorReporter->addError(QObject::tr("Bogus input values"));
			return "";
		}
	};

	QString result;
	if (!inputs.isNull()) {
		QString val;
		QFile file;
		file.setFileName(inputs);
		if (file.open(QIODevice::ReadOnly)) {
			val = file.readAll();
			file.close();
			QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
			QJsonObject object = document.object();
			for (const QString &name : object.keys()) {
				QJsonValue value = object[name];
				QString valueStr = jsValToStr(value);
				QString line("var " + name + " = " + valueStr + ";\n");
				result.append(line);
			}
		} else {
			mErrorReporter->addError(QObject::tr("Error: File couldn't open!"));
		}
	}
	return result;
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
		mRunning = false;
		emit completed();
	}
}
