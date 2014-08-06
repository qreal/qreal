#pragma once

#include <QtCore/QList>

#include "textLanguageParser/ast/range.h"
#include "textLanguageParser/details/token.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

/// Base class for all abstract syntax tree nodes.
class QRUTILS_EXPORT Node {
public:
	virtual ~Node() {}

	Connection const &start() const;
	Connection const &end() const;
	QList<Range> const &ranges() const;

	void connect(details::Token const &token);
	void connect(Node const &node);

private:
	/// Connection of a node is a list of ranges of all its subnodes.
	QList<Range> mRanges;

	static Connection const noConnection;
};

}
}
