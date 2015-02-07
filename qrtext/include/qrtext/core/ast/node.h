#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrtext/core/range.h"
#include "qrtext/core/lexer/token.h"

#include "qrtext/core/astVisitorInterface.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {
namespace ast {

/// Base class for all abstract syntax tree nodes.
class QRTEXT_EXPORT Node
{
public:
	Node();

	virtual ~Node();

	/// Returns start of a node.
	Connection const &start() const;

	/// Returns end of a node.
	Connection const &end() const;

	/// Returns list of all ranges associated with a node (ranges of its own lexemes and its sons, excluding comments).
	QList<Range> const &ranges() const;

	/// Add given token range to the list of node ranges.
	template<typename TokenType>
	void connect(Token<TokenType> const &token)
	{
		connect({token.range()});
	}

	/// Add ranges of the given node to the list of node ranges.
	void connect(QSharedPointer<Node> const &node);

	/// Returns true, if this node is an instance of NodeType (descendant or is NodeType).
	template<typename NodeType>
	bool is() const
	{
		return dynamic_cast<NodeType const * const>(this) != nullptr;
	}

	/// Returns all children of a node as a list. Shall be redefined in all descendants who have children, as it is used
	/// everywhere where we traverse a tree.
	virtual QList<QSharedPointer<Node>> children() const
	{
		return {};
	}

	/// Calls visit() method of the given visitor for all children and self.
	void acceptRecursively(AstVisitorInterface &visitor) const;

	/// Calls visit() method of the given visitor for self.
	/// The descendants should override this method to call corresponding visit() method overload.
	/// See 'visitor' design pattern (http://www.oodesign.com/visitor-pattern.html).
	virtual void accept(AstVisitorInterface &visitor) const;

	static int nodesCount() {
		return mNodesCount;
	}

private:
	void connect(QList<Range> const &ranges);

	/// Connection of a node is a list of ranges of all its subnodes and tokens.
	QList<Range> mRanges;

	/// Static object denoting absent connection (for example, for generated nodes).
	static Connection const noConnection;

	static int mNodesCount;
};

}
}

/// Helper function that casts pointer to SourceType to pointer to TargetType.
template<typename TargetType, typename SourceType>
inline QSharedPointer<TargetType> as(QSharedPointer<SourceType> const &node)
{
	return node.template dynamicCast<TargetType>();
}

/// Helper function that casts list of pointers to SourceType to list of pointers to TargetType.
template<typename TargetType, typename SourceType>
inline QList<QSharedPointer<TargetType>> as(QList<QSharedPointer<SourceType>> const &list)
{
	QList<QSharedPointer<TargetType>> result;
	for (const auto &item : list) {
		result << as<TargetType>(item);
	}

	return result;
}

/// Helper function that makes shared pointer to a given node.
inline QSharedPointer<core::ast::Node> wrap(core::ast::Node *node)
{
	return QSharedPointer<core::ast::Node>(node);
}

}
