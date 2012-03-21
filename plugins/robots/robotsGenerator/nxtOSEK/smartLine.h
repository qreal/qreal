#pragma once

#include <QString>
#include "../../../../qrkernel/ids.h"

namespace robots {
namespace generator {

//! Class for representing code line generated from Robot Language Diagram.
/*!
 * Contains information for formatting (indent level change) and connection with master object ID.
 */
class SmartLine {
public:
	//! Indent control type.
	enum IndentChangeType {
		increase, //!< increases indent WITHOUT this line
		decrease, //!< decreases indent WITH this line
		withoutChange, //!< doesn't change indent
		increaseDecrease //!< decreases indent only for this line
	};

	SmartLine(QString const &text, qReal::Id const &elementId, IndentChangeType indentLevelChange = withoutChange);

	QString text() const;
	qReal::Id elementId() const;
	IndentChangeType indentLevelChange() const;

	void setText(QString const &);
	void setElementId(qReal::Id const &);
	void setIndentChange(IndentChangeType);

private:
	QString mText;
	qReal::Id mElementId;
	IndentChangeType mIndentChange;
};
}
}
