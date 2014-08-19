#pragma once

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"

#include "qrtext/core/semantics/semanticAnalyzer.h"

#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/string.h"
#include "qrtext/lua/ast/tableConstructor.h"

namespace qrtext {
namespace lua {

class LuaInterpreter {
public:
//	explicit LuaInterpreter(QList<core::Error> &errors);

	template<typename T>
	T interpret(QSharedPointer<core::ast::Node> const &root, core::SemanticAnalyzer const &semanticAnalyzer)
	{
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
}
