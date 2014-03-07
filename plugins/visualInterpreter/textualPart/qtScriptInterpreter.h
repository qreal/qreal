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
