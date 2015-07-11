#pragma once

#include "qrtext/core/ast/node.h"
#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents string node.
class String : public Node
{
public:
	/// Constructor.
	/// @param identifierPart - identifier (complex identifier).
	/// @param textPart - constant text.
	String(const QSharedPointer<Node> &identifierPart
			, const QSharedPointer<Node> &textPart)
		: mIdentifierPart(identifierPart)
		, mTextPart(textPart)
	{
	}

	/// Returns identifier part.
	const QSharedPointer<Node> &identifierPart()
	{
		return mIdentifierPart;
	}

	/// Returns text part.
	const QSharedPointer<Node> &textPart()
	{
		return mTextPart;
	}

private:
	QSharedPointer<Node> mIdentifierPart;
	QSharedPointer<Node> mTextPart;
};

}
}
