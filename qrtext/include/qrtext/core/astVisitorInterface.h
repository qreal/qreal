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

	virtual void visit(ast::Node const &node)           { Q_UNUSED(node); }
	virtual void visit(ast::Expression const &node)     { Q_UNUSED(node); }
	virtual void visit(ast::BinaryOperator const &node) { Q_UNUSED(node); }
	virtual void visit(ast::UnaryOperator const &node)  { Q_UNUSED(node); }
};

}
}
