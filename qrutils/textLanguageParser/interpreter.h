#pragma once

#include <QtCore/QVariant>

#include "textLanguageParser/parserError.h"
#include "textLanguageParser/semanticAnalyzer.h"
#include "textLanguageParser/ast/nodes/node.h"

#include "textLanguageParser/ast/nodes/integerNumber.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class QRUTILS_EXPORT Interpreter {
public:
	explicit Interpreter(QList<ParserError> &errors);

	template<typename T>
	T interpret(QSharedPointer<ast::Node> const &root, SemanticAnalyzer const &semanticAnalyzer)
	{
		Q_UNUSED(root);
		Q_UNUSED(semanticAnalyzer);

		QVariant result;

		if (root->is<ast::IntegerNumber>()) {
			result = as<ast::IntegerNumber>(root)->stringRepresentation().toInt();
		}

		return result.value<T>();
	}
};

}
