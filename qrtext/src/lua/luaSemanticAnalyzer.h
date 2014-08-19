#pragma once

#include <QtCore/QScopedPointer>

#include "qrtext/core/semantics/semanticAnalyzer.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace lua {
namespace details {

class QRTEXT_EXPORT LuaSemanticAnalyzer : public core::SemanticAnalyzer {
public:
	LuaSemanticAnalyzer(QList<core::Error> &errors);

private:
	void analyzeNode(QSharedPointer<core::ast::Node> const &node) override;

	void constrainAssignment(QSharedPointer<core::ast::Node> const &operation
			, QSharedPointer<core::ast::Node> const &lhs, QSharedPointer<core::ast::Node> const &rhs);

	QSharedPointer<core::types::TypeExpression> mBoolean;
	QSharedPointer<core::types::TypeExpression> mFloat;
	QSharedPointer<core::types::TypeExpression> mInteger;
	QSharedPointer<core::types::TypeExpression> mNil;
	QSharedPointer<core::types::TypeExpression> mString;
};

}
}
}
