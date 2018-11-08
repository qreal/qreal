/* Copyright 2016-2017 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>

#include "trikKitInterpreterCommon/trikQtsInterpreter.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <twoDModel/engine/model/timeline.h>


Q_DECLARE_METATYPE(utils::AbstractTimer*)

const QString overrides = "script.random = brick.random;script.wait = brick.wait;script.time = brick.time;"
		"script.readAll = brick.readAll;script.timer = brick.timer;"
		"print = function() {var res = '';"
		"for(var i = 0; i < arguments.length; i++) {res += arguments[i].toString();}"
		"brick.log(res);return res;};"
		"script.system = function() {print('system is disabled in the interpreter');};";

trik::TrikQtsInterpreter::TrikQtsInterpreter(const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model)
	: mRunning(false), mBrick(model), mScriptRunner(mBrick, nullptr), mErrorReporter(nullptr)
{
	connect(&mBrick, &TrikBrick::error, this, &TrikQtsInterpreter::reportError);
	connect(&mBrick, &TrikBrick::warning, this, &TrikQtsInterpreter::reportWarning);

	auto atimerToScriptValue = [](QScriptEngine *engine, utils::AbstractTimer* const &in){
		return engine->newQObject(in);
	};
	auto atimerFromScriptValue = [](const QScriptValue &object, utils::AbstractTimer* &out){
		out = qobject_cast<utils::AbstractTimer*>(object.toQObject());
	};
	mScriptRunner.addCustomEngineInitStep([&atimerToScriptValue, &atimerFromScriptValue](QScriptEngine *engine){
		qScriptRegisterMetaType<utils::AbstractTimer*>(engine, atimerToScriptValue, atimerFromScriptValue);
	});
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
	mBrick.setCurrentInputs(inputs);
	QString newScript = overrides + "script.writeToFile = null;\n" + script;
	//qDebug() << newScript;
	mScriptRunner.run(newScript);
}

void trik::TrikQtsInterpreter::abort()
{
	//mScriptRunner.abort();
	mBrick.stopWaiting();
	Q_ASSERT(mScriptRunner.thread() == thread());
	QMetaObject::invokeMethod(&mScriptRunner, "abort", Qt::QueuedConnection); // just a wild test
	mRunning = false; // reset brick?
}

void trik::TrikQtsInterpreter::init()
{
	mBrick.init(); // very crucial. Maybe move into interpret methods?
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

void trik::TrikQtsInterpreter::reportWarning(const QString &msg)
{
	mErrorReporter->addWarning(msg);
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
	if (!inputs.isEmpty()) {
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
			mErrorReporter->addError(QObject::tr("Error: File %1 couldn't be opened!").arg(inputs));
		}
	}
	return result;
}

void trik::TrikQtsInterpreter::setRunning(bool running)
{
	mRunning = running;
}

void trik::TrikQtsInterpreter::setCurrentDir(const QString &dir)
{
	mBrick.setCurrentDir(dir);
}

QStringList trik::TrikQtsInterpreter::supportedRobotModelNames() const
{
	return {"TwoDRobotModelForTrikV62RealRobotModel", "TwoDRobotModelForTrikV6RealRobotModel"};
}

QStringList trik::TrikQtsInterpreter::knownMethodNames() const
{
	return mScriptRunner.knownMethodNames();
}

void trik::TrikQtsInterpreter::reinitRobotsParts()
{
	mBrick.reinitImitationCamera();
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
