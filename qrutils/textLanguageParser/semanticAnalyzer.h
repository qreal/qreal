#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QList>

#include <qrkernel/ids.h>

#include "textLanguageParser/parserError.h"
#include "textLanguageParser/ast/nodes/node.h"
#include "textLanguageParser/ast/nodes/expression.h"
#include "textLanguageParser/ast/nodes/identifier.h"

#include "textLanguageParser/types/typeExpression.h"
#include "textLanguageParser/types/boolean.h"
#include "textLanguageParser/types/float.h"
#include "textLanguageParser/types/integer.h"
#include "textLanguageParser/types/nil.h"
#include "textLanguageParser/types/string.h"
#include "textLanguageParser/types/any.h"

#include "textLanguageParser/types/generalizationsTable.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class QRUTILS_EXPORT SemanticAnalyzer {
public:
	SemanticAnalyzer(QList<ParserError> &errors);

	void analyze(QSharedPointer<ast::Node> const &root);

	QSharedPointer<types::TypeExpression> type(QSharedPointer<ast::Node> const &expression) const;

private:
	void assign(QSharedPointer<ast::Node> const &expression, QSharedPointer<types::TypeExpression> const &type);
	void unify(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs);

	void collect(QSharedPointer<ast::Node> const &node);
	void finalizeResolve(QSharedPointer<ast::Node> const &node);

	void constrainAssignment(QSharedPointer<ast::Node> const &operation
			, QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs);

	void constrain(QSharedPointer<ast::Node> const &operation
			, QSharedPointer<ast::Node> const &node, QList<QSharedPointer<types::TypeExpression>> const &types);

	void reportError(const QSharedPointer<ast::Node> &node, QString const &errorMessage);

	QHash<QSharedPointer<ast::Expression>, QSharedPointer<types::TypeExpression>> mTypes;
	QHash<QString, QSharedPointer<ast::Node>> mIdentifierDeclarations;

	QSharedPointer<types::TypeExpression> mBoolean;
	QSharedPointer<types::TypeExpression> mFloat;
	QSharedPointer<types::TypeExpression> mInteger;
	QSharedPointer<types::TypeExpression> mNil;
	QSharedPointer<types::TypeExpression> mString;

	QSharedPointer<types::TypeExpression> mAny;

	QList<ParserError> &mErrors;

	types::GeneralizationsTable mGeneralizationsTable;
};

}
