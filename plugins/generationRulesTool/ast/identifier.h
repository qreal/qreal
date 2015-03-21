#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents simple identifier node.
class Identifier : public Node
{
public:
	/// Constructor for Identifier.
	/// @param name - identifier name.
	Identifier(const QString &name)
		: mName(name)
	{
	}

	/// Returns identifier name.
	const QString &name() const {
		return mName;
	}

private:
	QString mName;
};

}
}
