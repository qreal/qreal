#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>

#include <qrkernel/ids.h>

#include "textLanguageParser/parserError.h"
#include "textLanguageParser/ast/nodes/node.h"
#include "textLanguageParser/ast/nodes/expression.h"
#include "textLanguageParser/types/typeExpression.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class QRUTILS_EXPORT SemanticAnalyzer {
public:
	void analyze(QSharedPointer<ast::Node> const &root, qReal::Id const &id, QString const &propertyName, QList<ParserError> &errors);

	types::TypeExpression const &type(QSharedPointer<ast::Expression> const &expression) const;

private:
	QList<ParserError> mErrors;

	QHash<QSharedPointer<ast::Expression>, types::TypeExpression> mTypes;
};

}
