#pragma once

#include <QtCore/QVariant>

#include "qrutils/textLanguageParser/parserError.h"
#include "qrutils/textLanguageParser/semanticAnalyzer.h"
#include "qrutils/textLanguageParser/ast/nodes/node.h"

#include "qrutils/textLanguageParser/ast/nodes/integerNumber.h"
#include "qrutils/textLanguageParser/ast/nodes/string.h"
#include "qrutils/textLanguageParser/ast/nodes/tableConstructor.h"

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

		if (root->is<ast::String>()) {
			result = as<ast::String>(root)->string();
		}

		if (root->is<ast::TableConstructor>()) {
			QStringList temp;
			for (auto node : as<ast::TableConstructor>(root)->initializers()) {
				temp << interpret<QString>(node->value(), semanticAnalyzer);
			}

			result = QVariant(temp);
		}

		return result.value<T>();
	}
};

}
