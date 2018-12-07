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
	void interpretScriptExercise(const QString &script, const QString &inputs);
	void abort();

	void init();
	void setErrorReporter(qReal::ErrorReporterInterface &errorReporter);
	bool isRunning() const;
	void setRunning(bool running);
	void setCurrentDir(const QString &dir);
	QStringList supportedRobotModelNames() const;
	QStringList knownMethodNames() const;

signals:
	void completed();

public slots:
	/// in some cases we need to reinit devices such as camera, etc..
	void reinitRobotsParts();

private slots:
	void scriptFinished(const QString &error, int scriptId);
	void reportError(const QString &msg);
	void reportWarning(const QString &msg);
	void reportLog(const QString &msg);

private:
	QString initInputs(const QString &inputs) const;

	//QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;
	bool mRunning;

	TrikBrick mBrick;
	trikScriptRunner::TrikScriptRunner mScriptRunner;
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
