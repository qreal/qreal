/* Copyright 2007-2015 QReal Research Group
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

#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QPair>
#include <QtCore/QHash>

#include "textCodeInterpreter.h"

namespace qReal {

/// Interprets textual part of semantics written on python in different python interpreter process, parses output
/// and sends it to the main system
class PythonInterpreter : public TextCodeInterpreter
{
	Q_OBJECT

public:
	explicit PythonInterpreter(QObject *parent, QString const &pythonPath = "python"
			, QString const &tempScriptPath = QDir().currentPath() + "/temp.py");
	~PythonInterpreter();

	/// Interpret python script
	bool interpret(QString const &code, CodeType const codeType);

	void terminateProcess();
	void continueStep();

	/// Delete generated python script file
	void deleteTempFile();

	void setPythonPath(QString const &path);
	void setTempScriptPath(QString const &path);

private slots:
	/// Read python interpreter std output
	void readOutput();

protected:
	bool startPythonInterpreterProcess();

	/// Parses interpreter std output and returns new values for element properties
	QHash<QPair<QString, QString>, QString> &parseOutput(QString const &output) const;

	QThread *mThread;
	QProcess *mInterpreterProcess;

	QString mPythonPath;
	QString mTempScriptPath;

	bool mPythonCodeProcessed;
};

}
