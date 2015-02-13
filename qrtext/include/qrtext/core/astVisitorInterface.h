#pragma once

#include <QtCore/QtGlobal>

namespace qrtext {
namespace core {

namespace ast {
class Node;
class Expression;
class BinaryOperator;
class UnaryOperator;
}

/// An interface for every entity that wants to operate with text language AST.
/// See 'visitor' design pattern (http://www.oodesign.com/visitor-pattern.html).
class AstVisitorInterface
{
public:
	virtual ~AstVisitorInterface() {}

	virtual void visit(const ast::Node &node)           { Q_UNUSED(node); }
	virtual void visit(const ast::Expression &node)     { Q_UNUSED(node); }
	virtual void visit(const ast::BinaryOperator &node) { Q_UNUSED(node); }
	virtual void visit(const ast::UnaryOperator &node)  { Q_UNUSED(node); }
};

}
}
