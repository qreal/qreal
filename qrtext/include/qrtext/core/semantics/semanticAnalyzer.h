#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QList>

#include <qrkernel/ids.h>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/ast/expression.h"
//#include "qrtext/core/ast/identifier.h"

#include "qrtext/core/types/typeExpression.h"
//#include "qrutils/textLanguageParser/types/boolean.h"
//#include "qrutils/textLanguageParser/types/float.h"
//#include "qrutils/textLanguageParser/types/integer.h"
//#include "qrutils/textLanguageParser/types/nil.h"
//#include "qrutils/textLanguageParser/types/string.h"
//#include "qrutils/textLanguageParser/types/any.h"

#include "qrtext/core/semantics/generalizationsTableInterface.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

class QRTEXT_EXPORT SemanticAnalyzer {
public:
	SemanticAnalyzer(QSharedPointer<GeneralizationsTableInterface> const &generalizationsTable, QList<Error> &errors);
	virtual ~SemanticAnalyzer();

	void analyze(QSharedPointer<ast::Node> const &root);

	QSharedPointer<types::TypeExpression> type(QSharedPointer<ast::Node> const &expression) const;

protected:
	void assign(QSharedPointer<ast::Node> const &expression, QSharedPointer<types::TypeExpression> const &type);

	void constrain(QSharedPointer<ast::Node> const &operation
			, QSharedPointer<ast::Node> const &node, QList<QSharedPointer<types::TypeExpression>> const &types);

	void unify(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs);

	void reportError(const QSharedPointer<ast::Node> &node, QString const &errorMessage);

	bool hasDeclaration(QString const &identifierName) const;
	QSharedPointer<ast::Node> declaration(QString const &identifierName) const;
	void addDeclaration(QString const &identifierName, QSharedPointer<ast::Node> const &declaration);

	QSharedPointer<types::TypeExpression> const &any();

	GeneralizationsTableInterface const &generalizationsTable() const;

private:
	void collect(QSharedPointer<ast::Node> const &node);
	void finalizeResolve(QSharedPointer<ast::Node> const &node);

	virtual void analyzeNode(QSharedPointer<ast::Node> const &node) = 0;

	QHash<QSharedPointer<ast::Expression>, QSharedPointer<types::TypeExpression>> mTypes;
	QHash<QString, QSharedPointer<ast::Node>> mIdentifierDeclarations;

	QSharedPointer<types::TypeExpression> mAny;

	QList<Error> &mErrors;

	QSharedPointer<GeneralizationsTableInterface> mGeneralizationsTable;
};

}
}
