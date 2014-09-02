#include "qrtext/src/lua/luaInterpreter.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/floatNumber.h"
#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/string.h"
#include "qrtext/lua/ast/tableConstructor.h"

using namespace qrtext::lua::details;
using namespace qrtext;

QVariant LuaInterpreter::interpret(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
{
	Q_UNUSED(semanticAnalyzer);

	QVariant result;

	/// @todo Integer and float literals may differ from those recognized in toInt() and toDouble().
	if (root->is<ast::IntegerNumber>()) {
		result = as<ast::IntegerNumber>(root)->stringRepresentation().toInt();
	}

	if (root->is<ast::FloatNumber>()) {
		result = as<ast::FloatNumber>(root)->stringRepresentation().toDouble();
	}

	if (root->is<ast::String>()) {
		result = as<ast::String>(root)->string();
	}

	if (root->is<ast::TableConstructor>()) {
		QStringList temp;
		for (auto node : as<ast::TableConstructor>(root)->initializers()) {
			temp << interpret(node->value(), semanticAnalyzer).value<QString>();
		}

		result = QVariant(temp);
	}

	if (root->is<ast::Assignment>()) {
		auto variable = as<ast::Assignment>(root)->variable();
		auto value = as<ast::Assignment>(root)->value();
		auto interpretedValue = interpret(value, semanticAnalyzer);

		if (variable->is<ast::Identifier>()) {
			auto name = as<ast::Identifier>(variable)->name();
			mIdentifierValues.insert(name, interpretedValue);
		}

		/// @todo Assignment is also possible to array slice, for example "a[1] = 1", or more complex case,
		///       "f(x)[1] = 1". Note that field access in form of "a.x = 1" is parsed as "a['x'] = 1", so no special
		///       handling is needed for that case.
	}

	if (root->is<ast::Identifier>()) {
		result = mIdentifierValues.value(as<ast::Identifier>(root)->name());
	}

	if (root->is<ast::FunctionCall>()) {
		auto name = as<ast::FunctionCall>(root)->name();
		auto parameters = as<ast::FunctionCall>(root)->arguments();

		QList<QVariant> actualParameters;
		for (auto parameter : parameters) {
			actualParameters << interpret(parameter);
		}

		result = mIntrinsicFunctions[name](actualParameters);
	}

	return result;
}

void LuaInterpreter::addIntrinsicFunction(QString const &name
		, std::function<QVariant(QList<QVariant> const &)> const &semantic)
{
	mIntrinsicFunctions.insert(name, semantic);
}
