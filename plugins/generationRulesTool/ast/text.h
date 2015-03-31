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
	Text(const QString &text)
		: mText(text)
	{
	}

	/// Returns value of a string (without quotes).
	const QString &text() {
		const QString &textWithoutQuotes = mText.remove('\'');
		return textWithoutQuotes;
	}

private:
	QString mText;
};

}
}
