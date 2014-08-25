#pragma once

#include <QtCore/QHash>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"

#include "qrtext/core/semantics/semanticAnalyzer.h"

namespace qrtext {
namespace lua {
namespace details {

class LuaInterpreter {
public:
//	explicit LuaInterpreter(QList<core::Error> &errors);

	QVariant interpret(QSharedPointer<core::ast::Node> const &root, core::SemanticAnalyzer const &semanticAnalyzer);

private:
	QHash<QString, QVariant> mIdentifierValues;
};

}
}
}
