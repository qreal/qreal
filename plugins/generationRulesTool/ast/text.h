#pragma once

#include <QtCore/QString>
#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents text node.
class Text : public Node
{
public:
	/// Constructor.
	/// @param string - value of a literal (with quotes).
	Text(QString const &text)
		: mText(text)
	{
	}

	/// Returns value of a string (with quotes).
	QString const &text() const {
		return mText;
	}

private:
	QString mText;
};

}
}
