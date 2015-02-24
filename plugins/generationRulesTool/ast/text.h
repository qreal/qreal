#pragma once

#include <QtCore/QString>
#include "node.h"

namespace simpleParser {
namespace ast {

class Text : public Node
{
public:
	/// Constructor.
	/// @param string - value of a literal (without quotes).
	Text(QString const &text)
		: mText(text)
	{
	}

	/// Returns value of a string (without quotes).
	QString const &text() const {
		return mText;
	}

private:
	QString mText;
};

}
}
