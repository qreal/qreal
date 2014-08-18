#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrtext/core/range.h"
#include "qrtext/core/lexer/token.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {
namespace ast {

/// Base class for all abstract syntax tree nodes.
class QRTEXT_EXPORT Node {
public:
	virtual ~Node() {}

	Connection const &start() const;
	Connection const &end() const;

	QList<Range> const &ranges() const;

	template<typename TokenType>
	void connect(lexer::Token<TokenType> const &token)
	{
		connect({token.range()});
	}

	void connect(QSharedPointer<Node> const &node);

	template<typename NodeType>
	bool is() const
	{
		return dynamic_cast<NodeType const * const>(this) != nullptr;
	}

	virtual QList<QSharedPointer<Node>> children() const
	{
		return {};
	}

private:
	void connect(QList<Range> const &ranges);

	/// Connection of a node is a list of ranges of all its subnodes and tokens.
	QList<Range> mRanges;

	static Connection const noConnection;
};

}
}

template<typename TargetType, typename SourceType>
inline QSharedPointer<TargetType> as(QSharedPointer<SourceType> const &node)
{
	return node.template dynamicCast<TargetType>();
}

template<typename TargetType, typename SourceType>
inline QList<QSharedPointer<TargetType>> as(QList<QSharedPointer<SourceType>> const &list)
{
	QList<QSharedPointer<TargetType>> result;
	for (auto item : list) {
		result << as<TargetType>(item);
	}

	return result;
}

inline QSharedPointer<core::ast::Node> wrap(core::ast::Node *node)
{
	return QSharedPointer<core::ast::Node>(node);
}

}
