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
	Identifier(QString const &name)
		: mName(name)
	{
	}

	/// Returns identifier name.
	QString const &name() const {
		return mName;
	}

private:
	QString mName;
};

}
}
