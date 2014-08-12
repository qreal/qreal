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
#include "textLanguageParser/types/typeEquation.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class QRUTILS_EXPORT SemanticAnalyzer {
public:
	SemanticAnalyzer(QList<ParserError> &errors);

	void analyze(QSharedPointer<ast::Node> const &root);

	QSharedPointer<types::TypeExpression> const &type(QSharedPointer<ast::Node> const &expression) const;

private:
	void assign(QSharedPointer<ast::Node> const &expression, QSharedPointer<types::TypeExpression> const &type);

	void collect(QSharedPointer<ast::Node> const &node);
	void resolveEquations();
	void finalizeResolve(QSharedPointer<ast::Node> const &node);

	void constrainAssignment(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs);
	void constrainCast(QSharedPointer<ast::Node> const &left, QSharedPointer<ast::Node> const &right);
	void constrainStructuralEquality(QSharedPointer<types::TypeExpression> const &left
			, QSharedPointer<types::TypeExpression> const &right);

	QHash<QSharedPointer<ast::Expression>, QSharedPointer<types::TypeExpression>> mTypes;
	QHash<QString, QSharedPointer<types::TypeExpression>> mIdentifierTypes;
	QHash<QSharedPointer<ast::Identifier>, QSharedPointer<ast::Node>> mIdentifierDeclarations;

	QSharedPointer<types::TypeExpression> mBoolean;
	QSharedPointer<types::TypeExpression> mFloat;
	QSharedPointer<types::TypeExpression> mInteger;
	QSharedPointer<types::TypeExpression> mNil;
	QSharedPointer<types::TypeExpression> mString;

	QSharedPointer<types::TypeExpression> mAny;

	QList<types::TypeEquation> mEquations;
	QList<ParserError> &mErrors;
};

}
