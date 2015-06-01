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

#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QHash>

namespace qReal {

/// Interprets text code reaction, parses output and sends it to the main system
class TextCodeInterpreter : public QObject
{
	Q_OBJECT

public:
	enum CodeType {
		initialization,
		applicationCondition,
		reaction
	};

	enum CodeLanguage {
		python,
		qtScript
	};

public:
	explicit TextCodeInterpreter(QObject *parent);

	/// Interpret text code script
	virtual bool interpret(QString const &code, CodeType const codeType) = 0;

signals:
	/// Emitted after parsing std output and has all properties changes
	void readyReadStdOutput(QHash<QPair<QString, QString>, QString> const &output
			, TextCodeInterpreter::CodeLanguage language);

	/// Raw error output from python interpreter
	void readyReadErrOutput(QString const &output);

protected:
	/// Parses interpreter std output and returns new values for element properties
	virtual QHash<QPair<QString, QString>, QString> &parseOutput(QString const &output) const;

	/// Moves along the output and accumulates properties changes
	void parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const;

	bool mApplicationConditionResult;
	bool mErrorOccured;
};

}
