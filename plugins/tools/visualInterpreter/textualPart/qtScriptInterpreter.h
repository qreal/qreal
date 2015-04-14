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

#include <QtCore/QPair>
#include <QtCore/QHash>
#include <QtScript/QScriptEngine>

#include "textCodeInterpreter.h"

namespace qReal {

/// Interprets textual part of semantics written on QtScript, parses output and sends it to the main system
class QtScriptInterpreter : public TextCodeInterpreter
{
	Q_OBJECT

public:
	explicit QtScriptInterpreter(QObject *parent);

	/// Interpret QtScript script
	bool interpret(QString const &code, CodeType const codeType);

protected:
	void processOutput(QString const &outputString);

	QScriptEngine mEngine;
};

}
