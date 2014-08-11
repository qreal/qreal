#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>

#include <qrkernel/ids.h>

#include "textLanguageParser/parserError.h"
#include "textLanguageParser/ast/nodes/node.h"
#include "textLanguageParser/ast/nodes/expression.h"
#include "textLanguageParser/ast/nodes/identifier.h"
#include "textLanguageParser/types/typeExpression.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class QRUTILS_EXPORT SemanticAnalyzer {
public:
	SemanticAnalyzer();

	void analyze(QSharedPointer<ast::Node> const &root, QList<ParserError> &errors);

	types::TypeExpression const &type(QSharedPointer<ast::Expression> const &expression) const;

private:
	QList<ParserError> mErrors;

	QHash<QSharedPointer<ast::Expression>, QSharedPointer<types::TypeExpression>> mTypes;
	QHash<QString, QSharedPointer<types::TypeExpression>> mIdentifierTypes;
	QHash<QSharedPointer<ast::Identifier>, QSharedPointer<ast::Node>> mIdentifierDeclarations;
};

}
